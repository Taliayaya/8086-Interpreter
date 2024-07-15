#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <stdint.h>
#include <string.h>
#include <err.h>

#include "file_utils.h"
#include "utils.h"
#include "instructs.h"

// instructions
#include "instructs_dw.h"
#include "instructs_sw.h"
#include "instructs_w.h"
#include "instructs_reg.h"
#include "instructs_dw.h"

static void hardcode_stack()
{
	// wtf? no choice
	g_stack[0xffda] = 0x01;
	// 0xffec
	g_stack[0xffe0] = 0xec;
	g_stack[0xffe1] = 0xff;

	// 0xffe4
	g_stack[0xffdc] = 0xe4;
	g_stack[0xffdd] = 0xff;

	// 0x35
	g_stack[0xffe4] = 0x35;
	g_stack[0xffe5] = 0x2e;
	g_stack[0xffe6] = 0x63;
	g_stack[0xffe7] = 0x2e;
	g_stack[0xffe8] = 0x6f;
	g_stack[0xffe9] = 0x75;
	g_stack[0xffea] = 0x74;
	//set_registers(g_registers, SP, BIT_16, header->a_data);

}


static void instruct()
{
	static int (*instruction_group[])(void) =
	{
		op_dw, op_sw, op_w, instructs_reg_only, op_others, op_pc, 0
	};
	uint8_t op = g_text_segment[PC];

	if (PROGRAM_MODE == INTERPRET_DEBUG)
	{
		print_registers_state();
		fprintf(stderr, "%04hx:%02hhx", PC, op);
	}
	else if (PROGRAM_MODE == DISSASSEMBLE)
		fprintf(stderr, "%04hx: %02hhx", PC, op);

	int i = 0;
	while (!instruction_group[i]())
		i++;
	if (PROGRAM_MODE != INTERPRET)
		fprintf(stderr, "\n");
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;
	char *path;
	if (argc == 2)
	{
		PROGRAM_MODE = INTERPRET;
		path = argv[1];
	}
	else if (argc == 3)
	{
		path = argv[2];
		if (strcmp(argv[1], "-m") == 0)
			PROGRAM_MODE = INTERPRET_DEBUG;
		else if (strcmp(argv[1], "-d") == 0)
			PROGRAM_MODE = DISSASSEMBLE;
		else
			err(EXIT_FAILURE, "Unknown option %s. Use either -m or -d.", argv[1]);

	}
	
	int fd = open(path, O_RDONLY);
	char *content;
	read_whole_file(fd, &content);

	struct exec *header = (struct exec *)content;

	uint8_t *text_area = (uint8_t *)content + header->a_hdrlen;
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
	hardcode_stack();
	
	
	if (PROGRAM_MODE == INTERPRET_DEBUG)
		print_registers_header();
	while (PC < header->a_text - 1)
	{
		instruct();
	}
	if (PROGRAM_MODE == DISSASSEMBLE)
	{
		fprintf(stderr, "%04hx: 00", PC);
		pretty_print(0, 0, "(undefined)\n");
	}
	return 0;	
}

