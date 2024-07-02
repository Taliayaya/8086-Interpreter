#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <stdint.h>

#include "file_utils.h"
#include "utils.h"
#include "instructs.h"

// instructions
#include "instructs_dw.h"
#include "instructs_sw.h"
#include "instructs_w.h"
#include "instructs_reg.h"
#include "instructs_dw.h"


void instruct()
{
	uint8_t op = g_text_segment[PC];

	print_registers_state();
	printf("%04lx:%02hhx", PC, op);
	if (op_dw())
		return;
	else if (op_sw())
		return;
	else if (op_w())
		return;
	else if (instructs_reg_only())
		return;
	else if (op_others())
		return;
	else if (op_pc())
		return;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	
	char *path = argv[1];
	int fd = open(path, O_RDONLY);
	char *content;
	read_whole_file(fd, &content);

	struct exec *header = (struct exec *)content;

	uint8_t *text_area = content + header->a_hdrlen;
	// INIT
	g_text_segment = text_area;
	PC = 0;

	g_memory = malloc(STACK_CAPACITY);
	// copy the data area into the memory
	// maybe make it "static" / readonly?
	uint8_t *data_area = text_area + header->a_text;
	for (int i = 0; i < header->a_data; ++i)
		g_memory[i] = data_area[i];

	g_stack = g_memory; // same memory space
	// wtf? no choice
	g_stack[0xffda] = 0x01;
	// 0xffec
	g_stack[0xffe0] = 0xec;
	g_stack[0xffe1] = 0xff;
	//set_registers(g_registers, SP, BIT_16, header->a_data);

	printf("sizeof %zu\n", sizeof(struct flags));

	print_registers_header();
	while (PC < header->a_text - 1)
	{
		instruct();
	}
	//printf("%04hx: 00		(undefined)\n", PC);
	print_registers_state();
	printf("%04hx:not support rom=0 in writeEA\n", PC);

	printf("\n");
	for (int32_t i = 0; i < header->a_data; ++i)
		printf("%c", data_area[i]);


	return 0;	
}
