#include "syscalls.h"
#include <unistd.h>

#include <sys/syscall.h>

static fn_syscall_print syscall_print[NCALLS] = 
{
	sys_exit_1, 
	0, 
	0, 
	sys_write_4, 
	0
};

void syscall_hat(uint8_t *memory, uint16_t registers[8])
{
	uint16_t addr = registers[BX];
	message *m = (message *)(memory + addr);

	//printf("m_type: %i\n", m->m_type);
	//printf("m_source: %i\n", m->m_source);

	syscall_print[m->m_type - 1](memory, m);
}

void sys_exit_1(uint8_t *memory, message *m)
{
	int status = m->m1_i1;
	printf("<exit(%i)>\n", status);
	exit(status);
}

void sys_write_4(uint8_t *memory, message *m)
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
}

