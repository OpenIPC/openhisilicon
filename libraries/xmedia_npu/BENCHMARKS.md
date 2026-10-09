# libxmedia_npu: cross-check against the SDK

The open stack (`open_npu.ko` from `kernel/npu/gk7205v500`, `libxmedia_npu.so`
from here) against the XMedia SPC020 one (the SDK's `npu.o` relinked as
`open_npu.ko`, and its `libxmedia_npu.so`), with the SDK's `libxmedia_cl.so`
graph runtime on top of both, as it ships.

## Test conditions

- Board: GK7205V510 (Cortex-A7 @ 1 GHz, 128 MB), OpenIPC, kernel 4.9.37,
  `HZ=100`, no cpufreq/cpuidle (fixed clocks), NPU clock 450 MHz
- Models: the SDK's `sample/npu/xmm` graphs, run with their own inputs and
  golden outputs:
  - `normal`: person detector, 640x360 NV12, 731 KB `.xmm`, 3 outputs
  - `zero_copy`: 929 KB `.xmm`, 3 outputs
- `test/npu_bench`: loads the graph, one untimed run, then N timed
  `xmedia_cl_graph_process()` calls (wall clock around the call), each
  checked byte for byte against the golden outputs
- All four kernel module x library combinations, 300 runs each

## Results

### Correctness

| Check | Result |
|---|---|
| Golden mismatches, 2400 inferences (4 combinations x 2 models x 300) | 0 |
| Outputs of the last run, md5 across the 4 combinations | identical, both models |
| ioctl stream (`test/ioctl_trace.so`), vendor vs open library, `normal` x 4 | identical commands and payloads; only userspace addresses of MMZ mappings differ |
| DFX abort (`test/npu_abort_test`: 0xff command stream in a 64 KB window), both libraries | state 5, report 0x50000 (`rwddr_out_of_bound_err`, `dma_r_resp_p_err`); reset 0; next inference matches golden |

### Latency (ms per inference, majestic stopped, 3 rounds x 300 runs)

| Kernel module | Library | `normal` min | `normal` p50 | `zero_copy` min | `zero_copy` p50 |
|---|---|---|---|---|---|
| SDK `npu.o` | SDK | 6.059 | 17.49 | 5.720 | 11.39 |
| SDK `npu.o` | open | 6.058 | 17.49 (one round 6.08) | 5.717 | 11.39 |
| open | SDK | 6.056 | 17.49 | 5.716 | 11.39 |
| open | open | 6.057 | 17.49 | 5.716 | 11.39 (one round 5.73) |

Every combination has the same floor (6.06 ms / 5.72 ms, the NPU itself)
and the same distribution above it: latencies sit on steps about 10 ms
apart (6.1, 11.4, 17.5, 21.4, 31.4 ms), and an occasional round stays on the
first step throughout. With majestic streaming 1080p H.265 the steps are
the same, and the round-to-round spread is wider, for all four alike.

The steps come from `libxmedia_cl`, not from either stack under test: it
runs the NPU from a worker thread that blocks in `xmedia_npu_wait_job()`
and hands the result back to the caller through a condition variable, and
with `HZ=100` its wake-ups land on timer ticks. Load time (open device,
parse and upload the graph) is 715-790 ms in every combination.

## Reproduce

```sh
# on the build host, with the firmware toolchain on PATH
CC=arm-openipc-linux-musleabi-gcc
INC=-I../../kernel/include/gk7205v500
$CC -O2 $INC -o npu_bench test/npu_bench.c -lxmedia_cl -lxmedia_npu
$CC -O2 $INC -o npu_abort_test test/npu_abort_test.c -lxmedia_npu
$CC -O2 -shared -fPIC -o ioctl_trace.so test/ioctl_trace.c -ldl

# on the camera; <dir> holds neuron_network.xmm, input_data*.bin and
# output_data*.bin, as in the SDK's sample/npu/xmm/*/data
./npu_bench <dir> 300 [dump-dir]
LD_PRELOAD=./ioctl_trace.so IOCTL_TRACE=/tmp/trace.log ./npu_bench <dir> 3
./npu_abort_test && ./npu_bench <dir> 50
```

Swap libraries with `LD_LIBRARY_PATH` and modules with `rmmod open_npu;
insmod ...`; both can change with the camera up.
