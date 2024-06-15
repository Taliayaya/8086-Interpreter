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
	printf("%04lx:%02hhx", index, op);
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
	g_g_text_segment = text_area;
	PC = 0;

	g_memory = text_area + header->a_text;

	printf("sizeof %zu\n", sizeof(struct flags));

	print_registers_header();
	while (PC < header->a_text - 1)
	{
		instruct();
	}
	printf("%04lx: 00		(undefined)\n", PC);
	uint8_t *data_area = text_area + header->a_text;
	printf("\n");
	for (int32_t i = 0; i < header->a_data; ++i)
		printf("%c", data_area[i]);


	return 0;	
}
