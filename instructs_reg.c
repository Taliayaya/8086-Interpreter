#include "instructs_reg.h"

#define DEFAULT_W	1

typedef int (*reg_instruction)(uint8_t, uint8_t);

int instructs_reg_only(uint8_t **text_segment)
{
	reg_instruction instructions[] = {
		op_push_1,
		op_pop_1,
		op_xchg_1,
		op_inc_1,
		op_dec_1
	};
	size_t length = sizeof(instructions) / sizeof(reg_instruction);

	uint8_t byte1, op, reg;
	byte1 = **text_segment;
	op = byte1  & 0b11111000;
	reg = byte1 & 0b00000111;


	size_t index = 0;
	while (index < length &&
		!instructions[index](op, reg))
	{
		index++;
	}

	if (index < length)
	{
		*text_segment += 1;
		return 1;
	}
	return 0;
}

// reg instructions (0 1 0 1 0 reg)

void print_reg(char *op_name, uint8_t reg, uint8_t w)
{
	printf("	%s %s\n", op_name, get_reg(reg, w));
}

int op_push_1(uint8_t op, uint8_t reg)
{
	if (op == OP_PUSH_1)
	{
		print_reg("	push", reg, DEFAULT_W);
		return 1;
	}
	else
		return 0;
}

int op_pop_1(uint8_t op, uint8_t reg)
{
	if (op == OP_POP_1)
	{
		print_reg("	pop", reg, DEFAULT_W);
		return 1;
	}
	else
		return 0;
}


int op_xchg_1(uint8_t op, uint8_t reg)
{
	if (op == OP_XCHG_1)
	{
		printf("		xchg %s, ax\n", get_reg(reg, DEFAULT_W));
		return 1;
	}
	else
		return 0;
}

int op_inc_1(uint8_t op, uint8_t reg)
{
	if (op == OP_INC_1)
	{
		print_reg("	inc", reg, DEFAULT_W);
		return 1;
	}
	else
		return 0;
}

int op_dec_1(uint8_t op, uint8_t reg)
{
	if (op == OP_DEC_1)
	{
		print_reg("	dec", reg, DEFAULT_W);
		return 1;
	}
	else
		return 0;
}

