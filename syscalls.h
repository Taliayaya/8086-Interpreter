/* Emulate the syscalls of a minix os */

#include "utils.h"

#include "callnr.h"
#include "type.h"

typedef void (*fn_syscall_print)(uint8_t *, message *);

void syscall_hat(uint8_t *memory, uint16_t registers[8]);

void sys_exit_1(uint8_t *memory, message *m);
void sys_write_4(uint8_t *memory, message *m);


