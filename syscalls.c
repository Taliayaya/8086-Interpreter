#include "syscalls.h"
#include <unistd.h>

#include <sys/syscall.h>

static const fn_syscall_print syscall_print[NCALLS] = 
{
	sys_exit_1, 0, 0, sys_write_4, 	0,
	0, 			0, 0, 0, 			0, // 10
	0, 			0, 0, 0, 			0,
	0, 			sys_brk_17, 0, 0, 	0, // 20
	0, 			0, 0, 0, 			0,
	0, 			0, 0, 0, 			0, // 30
	0, 			0, 0, 0, 			0,
	0, 			0, 0, 0, 			0, // 40
	0, 			0, 0, 0, 			0,
	0, 			0, 0, 0, 			0, // 50
	0,			0, 0, sys_ioctl_54, 0,
};

void syscall_hat(uint8_t *memory, uint16_t registers[8])
{
	uint16_t addr = registers[BX];
	message *m = (message *)(memory + addr);

	//printf("m_type: %i\n", m->m_type);
	//printf("m_source: %i\n", m->m_source);

	syscall_print[m->m_type - 1](memory, registers, m);
}

void sys_exit_1(uint8_t *memory, uint16_t registers[8], message *m)
{
	int status = m->m1_i1;
	printf("<exit(%i)>\n", status);
	exit(status);
}

void sys_write_4(uint8_t *memory, uint16_t registers[8], message *m)
{
	uint16_t fd, nbytes, buffer;
	fd = m->m1_i1;
	nbytes = m->m1_i2;
	buffer = m->m1_p1;

	printf("<write(%i, 0x%04hx, %i)",
		fd, buffer, nbytes);
	for (int i = 0; i < nbytes; ++i)
		printf("%c", g_memory[buffer + i]);
	printf(" => %i>\n", nbytes);

	// AX cleared
	registers[AX] = 0;
	// set nbytes to [BX + 2]
	set_memory(memory, registers[BX] + 2, BIT_16, nbytes);
}

void sys_brk_17(uint8_t *memory, uint16_t registers[8], message *m)
{
	uint16_t addr;
	addr = m->m1_p1;
	int8_t ret = 0;

	printf("<brk(0x%04hx) => %hhx>\n", addr, ret);

	// AX cleared
	registers[AX] = 0;
	set_memory(memory, registers[BX] + 2, BIT_16, ret);
	set_memory(memory, registers[BX] + 18, BIT_16, addr);
}

void sys_ioctl_54(uint8_t *memory, uint16_t registers[8], message *m)
{
	uint16_t fd = m->TTY_LINE;
	uint16_t request = m->TTY_REQUEST;
	uint16_t data = m->ADDRESS;
	printf("<ioctl(%hx, 0x%hx, 0x%hx)>\n", fd, request, data);
	//ioctl(fd, request, g_memory + data);
    //return(_syscall(FS, IOCTL, &m));

	registers[AX] = 0; // encore??
	set_memory(memory, registers[BX] + 2, BIT_16, 0xffea);
}
