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

// kind memory for myself
//static void hardcode_stack()
//{
//	// wtf? no choice
//	g_stack[0xffda] = 0x01;
//	// 0xffec
//	g_stack[0xffe0] = 0xec;
//	g_stack[0xffe1] = 0xff;
//
//	// 0xffe4
//	g_stack[0xffdc] = 0xe4;
//	g_stack[0xffdd] = 0xff;
//
//	// 0x35
//	g_stack[0xffe4] = 0x35;
//	g_stack[0xffe5] = 0x2e;
//	g_stack[0xffe6] = 0x63;
//	g_stack[0xffe7] = 0x2e;
//	g_stack[0xffe8] = 0x6f;
//	g_stack[0xffe9] = 0x75;
//	g_stack[0xffea] = 0x74;
//	//set_registers(g_registers, SP, BIT_16, header->a_data);
//
//}

/**
  * Push the argc, argv & env in the stack
  */
static void init_stack(int argc, char **argv)
{
	char *env = "PATH=/bin:/usr/bin";

	// length of env + \0
	uint16_t len = strlen(env) + 1;

	for (int i = 0; i < argc; ++i)
		len += strlen(argv[i]) + 1;
	
	// if not even, add null to stack
	//if (len % 2 != 0)
	//	push_stack('\0', BIT_8);
	
	// push env in reverse order
	push_stack('\0', BIT_8);
	for (int i = strlen(env)- 1; i >= 0; --i)
		push_stack(env[i], BIT_8);

	// get addr
	uint16_t env_addr = g_registers[SP];
	uint16_t arg_addr[argc];

	// push each argv in reverse order and fetch their SP
	for (int i = argc - 1; i >= 0; --i)
	{
		push_stack('\0', BIT_8);
		for (int j = strlen(argv[i]) - 1; j >= 0; --j)
			push_stack(argv[i][j], BIT_8);
		arg_addr[i] = g_registers[SP];
	}

	// separator
	push_stack(0, BIT_16);
	// push env addr
	push_stack(env_addr, BIT_16);
	// separator
	push_stack(0, BIT_16);

	// push each argv addr (in reverse order)
	for (int i = argc - 1; i >= 0; --i)
		push_stack(arg_addr[i], BIT_16);
	push_stack(argc, BIT_16);
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
	while (instruction_group[i] && !instruction_group[i]())
		i++;
	if (PROGRAM_MODE != INTERPRET)
		fprintf(stderr, "\n");
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;
	char *path;
	if (argv[1][0] != '-')
	{
		PROGRAM_MODE = INTERPRET;
		path = argv[1];
	}
	else
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

	// read input file
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

	int _argc = PROGRAM_MODE == INTERPRET ? argc - 1 : argc - 2;
	char **_argv = PROGRAM_MODE == INTERPRET ? argv + 1 : argv +  2;

	init_stack(_argc, _argv);

	
	
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

