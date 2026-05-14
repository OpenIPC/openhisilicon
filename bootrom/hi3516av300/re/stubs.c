/*
 * Empty stub frontier — every named function reachable from _entry
 * in the av300 mask-ROM is now reversed in bootloader.c. The only
 * entry below is a trivial alias to resolve a placeholder name used
 * during reversal.
 */

extern int media_init_alt(void);
int alt_media_init(void) { return media_init_alt(); }
