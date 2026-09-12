wdt.c is the driver. wdt_init.c is how it gets bound, and between them they
serve every generation whose watchdog is the SP805 at some address: the V4
parts (hi3516ev200/ev300, gk7205v200/v300), hi3516cv500, and hi3516cv300. The
per-chip kbuild supplies WDT_BASE, and:

	WDT_OSAL_HI                     the OSAL API lives in hi_osal.h here,
	                                not osal.h (V3 and hi3516cv500 trees)
	WDT_OSAL_NO_KTHREAD_STOP_FLAG   the V3 osal_kthread_destory() takes the
	                                task alone and always stops it
	WDT_NO_OF                       no watchdog node in the device tree, so
	                                the module registers on load and maps
	                                WDT_BASE itself instead of binding as a
	                                platform driver
	WDT_SCTL_BASE                   the 3 MHz clock is gated in the system
	                                controller at this address and has to be
	                                ungated before the counter runs

Two directories beside this one keep their own copy, for reasons that are not
cosmetic. hi3516cv6xx and hi3519dv500 are a later vendor generation: they take
an optional FIQ feeding path and an hrtimer, and their OSAL is ot_osal.h with a
command-list ioctl dispatch. hi3516cv100 and hi3519v101 do not compile from
source at all today -- their kbuild links a vendor .o -- so what is here is
reference, and nothing in it reaches a board until that changes.

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
