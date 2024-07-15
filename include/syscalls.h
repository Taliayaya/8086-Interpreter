/* Emulate the syscalls of a minix os */

#include <string.h>

#include "utils.h"

#include "minix/callnr.h"
#include "minix/type.h"
#include "minix/com.h"

typedef void (*fn_syscall_print)(int8_t *, uint16_t[8], message *);

void syscall_hat(int8_t *memory, uint16_t registers[8]);

void sys_exit_1(int8_t *memory, uint16_t registers[8], message *m);
void sys_write_4(int8_t *memory, uint16_t registers[8], message *m);
void sys_brk_17(int8_t *memory, uint16_t registers[8], message *m);
void sys_ioctl_54(int8_t *memory, uint16_t registers[8], message *m);
