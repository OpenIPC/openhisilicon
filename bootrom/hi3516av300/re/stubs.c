/*
 * Link-only stubs for callees of main() at addresses 0x04001128,
 * 0x04002284, 0x04002368, 0x040023c4, 0x0400249c, 0x040025d4,
 * 0x0400105c, 0x040051bc, 0x040051e8, 0x040051f8, 0x04002cb4.
 *
 * Kept in a separate translation unit so -O2 in bootloader.c cannot
 * see through them at call sites — that preserves main()'s real
 * conditional structure in re.elf. Each stub will be replaced with
 * its reverse-engineered body in a later PR; main() does not need to
 * change when that happens.
 */

typedef unsigned int u32;
typedef unsigned int size_t;

int  initial_memory_setup(void)         { return 0; }
void timer_start(void)                  {}
void timer_stop(void)                   {}
void timer_reset_counter(void)          {}
int  fastboot_update(void)              { return 0; }
int  media_update(void)                 { return 0; }
int  is_update_mode(void)               { return 0; }
int  config_use_flash(void)             { return 0; }
void timer_wait_seconds(unsigned n)     { (void)n; }
int  bootrom_secure_check(void)         { return 0; }
int  bootrom_alt_media(void)            { return 0; }
