/* Emulate the syscalls of a minix os */

#include <string.h>

#include "utils.h"

#include "callnr.h"
#include "type.h"
#include "com.h"

typedef void (*fn_syscall_print)(uint8_t *, uint16_t[8], message *);

void syscall_hat(uint8_t *memory, uint16_t registers[8]);

void sys_exit_1(uint8_t *memory, uint16_t registers[8], message *m);
void sys_write_4(uint8_t *memory, uint16_t registers[8], message *m);
void sys_brk_17(uint8_t *memory, uint16_t registers[8], message *m);
void sys_ioctl_54(uint8_t *memory, uint16_t registers[8], message *m);
