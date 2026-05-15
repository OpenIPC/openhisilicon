/*
 * One alias to resolve a working name used during reversal — the
 * actual function is media_init_alt in bootloader.c.
 */

extern int media_init_alt(void);
int alt_media_init(void) { return media_init_alt(); }
