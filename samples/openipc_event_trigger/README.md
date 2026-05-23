# openipc_event_trigger — correlate an external GPIO pulse with the nearest frame

## What this is for

Industrial-style cameras have a **hardware trigger IN** pin: assert a
voltage on it and the camera captures *exactly one* frame, synchronously
with the pulse. PLC-driven inspection, photoeye-gated capture, strobed
illumination — they all rely on it.

OpenIPC HiSilicon cameras can't do that directly: the sensor's
`XVS_IN` / `TRIG` pin is essentially never broken out on commodity IP-
camera boards, and the per-sensor slave-mode driver work isn't there.
The camera keeps free-running at its configured frame rate.

**This tool gives you the next-best thing:** correlate the moment a
GPIO pulse arrives on the camera board with the nearest sensor
frame-start, so you can answer "which frame was the camera capturing
when the trigger fired?". Useful for:

- PLC / photoeye event marking — "this frame coincides with the part
  crossing the photo-detector"
- Strobed illumination diagnostics — verify your flash and sensor are
  in phase even though they aren't hardware-locked
- Multi-camera event-ordering when you can wire all cameras' trigger
  lines to the same external source

The trigger doesn't *cause* a capture; the camera was going to capture
that frame anyway. The tool just tells you which frame the trigger
landed on.

## How it works

1. Configures a SoC GPIO as input + rising-edge IRQ via `/sys/class/gpio/gpio<N>/`.
2. Subscribes to `/dev/openipc-frame-ts` for `MIPI_FS` events.
3. `poll()` both. On a GPIO rising edge it samples `CLOCK_REALTIME`
   immediately, then matches against:
   - the **most recent FS event** (the frame whose capture window the
     trigger fell into); and
   - the **next FS event** (the first frame that starts fully after
     the trigger).

The chrdev side is built around the kernel module shipped by
[`openhisilicon` PR #155 + #178][chrdev]. The GPIO side uses the
classic `/sys/class/gpio` interface with `edge=rising` + `poll(POLLPRI)`
— supported by every HiSi kernel we ship.

## Accuracy

The trigger timestamp has the usual kernel→user transit jitter:
typically a few µs on a quiet system, low single-digit ms under load.
For PLC-class event marking that's fine; for sub-µs you'd need a
kernel-side correlator (not provided here).

The FS event timestamps come from inside the kernel IRQ handler and
are NTP-disciplined `CLOCK_REALTIME`, so the correlation itself is
limited by NTP quality, not by this tool.

## Usage

```sh
# Build
cd samples/openipc_event_trigger && make

# Run on the camera
scp openipc_event_trigger root@camera:/tmp/
ssh root@camera /tmp/openipc_event_trigger -g 16        # default chn 0, no timeout
ssh root@camera /tmp/openipc_event_trigger -g 16 -t 30  # auto-exit after 30 s
ssh root@camera /tmp/openipc_event_trigger -g 16 -c 0x3 # channels 0+1
```

Pick a GPIO that's physically reachable on your board. The IR-cut
filter pin is the universal candidate for hand-soldered testing — it's
wired on essentially every IP camera.

## Output

One line per trigger pulse:

```
trigger=1779471569123456789 last_fs={seq=412 pts=1234567 wall=1779471569100000000 dt=-23.456 ms} next_fs={seq=413 pts=1267900 wall=1779471569133333333 dt=+9.876 ms}
```

- `trigger` — `CLOCK_REALTIME` nanoseconds when the GPIO edge was seen.
- `last_fs` — FS event the trigger fell *within*. `dt` is negative
  (trigger arrived this much after the frame started).
- `next_fs` — first FS event that started *after* the trigger.
  `dt` is positive (trigger arrived this much before the next frame).

The camera-side wall clocks are exact-as-kernel-knows; the trigger
timestamp jitter is described in "Accuracy" above.

## Limitations

- **Not a true hardware trigger IN.** The camera doesn't start
  capturing on the pulse — it was already capturing at its
  free-running rate.
- **Requires a SoC GPIO physically reachable on the camera board.**
  Sealed consumer IP-camera enclosures may not expose any extra pins;
  the IR-cut filter GPIO is the universal one and can be repurposed
  during testing.
- **One trigger at a time.** If two GPIO pulses arrive within one
  frame period, the tool emits the first with its `next_fs` filled and
  the second with `next_fs=(pending)`. Tighten your pulse spacing or
  fork the tool to widen the pending queue.

## Compared to the strobe-out sysfs

[Strobe-out](../../kernel/openipc_frame_ts/README.md#strobe-out-gpio-driven-by-frame-events)
goes the other direction: the camera drives an external GPIO from its
sensor IRQs (for flash / ring-light gating). This tool is the input
counterpart. The two can coexist — pick different GPIOs for each.

[chrdev]: ../../kernel/openipc_frame_ts/README.md
