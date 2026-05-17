# openipc_frame_ts_test

Userspace test for `/dev/openipc-frame-ts`. Opens the chrdev, reads events,
and (optionally) prints a per-channel jitter summary.

## Build

```sh
make CC=arm-openipc-linux-musleabi-gcc
```

## Run

```sh
./openipc_frame_ts_test            # one log line per event
./openipc_frame_ts_test -s         # per-second summary only
./openipc_frame_ts_test -c 0x1     # channel 0 only
```

On a Hi3516EV300 with a 30 fps sensor, expect:

```
chn0: n=1800 mean=33.333 ms min=33.250 ms max=33.420 ms sigma=24 us
```

See `kernel/openipc_frame_ts/README.md` for the kernel-side module design.
