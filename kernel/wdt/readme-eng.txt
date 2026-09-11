This module supports two loading paramaters: default_margin, nodeamon. Their means are as followed:
	(1) default_margin: the timmer margin(in second) the watchdog runs at until a user program sets its own. Its default value is 60. It should be greater than 0, and it will be set to 60 when it is out of range.
	(2) nodeamon:  this value decides whether to create a kernel thread to feed the watchdog automatically. A kernel thread to feed the watchdog is created when it is 0, or there is no thread to be created. Its default value is 0. The thread covers the window between module load and the first open of the device, and stops for good once a user program has opened it.

The device follows the Linux watchdog API (Documentation/watchdog/watchdog-api.txt):
opening /dev/watchdog starts the timer; WDIOC_KEEPALIVE or any write restarts
it; WDIOC_SETTIMEOUT takes the margin in seconds and reports back the one
programmed; and the margin is the time to the reset, not to the first of the
two counter expiries the SP805 needs. Writing "V" before closing stops the
timer, as WDIOF_MAGICCLOSE advertises. A close without it leaves the timer
running and unfed, so the board resets within the margin -- including the
close the kernel performs for a process that has crashed, which is the whole
point of the device.
