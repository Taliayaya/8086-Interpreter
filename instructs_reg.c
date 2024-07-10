#include "instructs_reg.h"

#define DEFAULT_W	1

typedef int (*reg_instruction)(uint8_t, uint8_t);

int instructs_reg_only()
{
	static reg_instruction instructions[] = {
		op_push_1,
		op_pop_1,
		op_xchg_1,
		op_inc_1,
		op_dec_1
	};
	static size_t length = sizeof(instructions) / sizeof(reg_instruction);

	uint8_t byte1, op, reg;
	byte1 = g_text_segment[PC];
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
		PC += 1;
		return 1;
	}
	return 0;
}

// reg instructions (0 1 0 1 0 reg)

void print_reg(char *op_name, uint8_t reg, uint8_t w)
{
	char instr[32];
	sprintf(instr, "%s %s", op_name, get_reg(reg, w));
	pretty_print(PC + 1, 0, instr);
}

int op_push_1(uint8_t op, uint8_t reg)
{
	if (op == OP_PUSH_1)
	{
		print_reg("+push", reg, DEFAULT_W);
		push_reg_stack(reg, BIT_16);
		return 1;
	}
	else
		return 0;
}

int op_pop_1(uint8_t op, uint8_t reg)
{
	if (op == OP_POP_1)
	{
		print_reg("+pop", reg, DEFAULT_W);
		pop_reg_stack(reg, BIT_16);
		return 1;
	}
	else
		return 0;
}


int op_xchg_1(uint8_t op, uint8_t reg)
{
	if (op == OP_XCHG_1)
	{
		char instr[32];
		sprintf(instr, "+xchg %s, ax\n", get_reg(reg, DEFAULT_W));
		pretty_print(PC + 1, 0, instr);

		uint16_t ldata, rdata;
		ldata = get_registers(g_registers, reg, BIT_16);
		rdata = get_registers(g_registers, AX, BIT_16);
		set_registers(g_registers, reg, BIT_16, rdata);
		set_registers(g_registers, AX,  BIT_16, ldata);
		return 1;
	}
	else
		return 0;
}

int op_inc_1(uint8_t op, uint8_t reg)
{
	if (op == OP_INC_1)
	{
		print_reg("+inc", reg, DEFAULT_W);
		uint16_t data;
		data = get_registers(g_registers, reg, BIT_16);
		set_registers(g_registers, reg, BIT_16, data + 1);
		// do not touch CF
		// OF SF ZF AF
		g_flags.ZF = (data + 1) == 0;
		g_flags.OF = data == 0xFFFF;
		g_flags.SF = ((data + 1) & 0x8000) == 0x8000;
		return 1;
	}
	else
		return 0;
}

int op_dec_1(uint8_t op, uint8_t reg)
{
	if (op == OP_DEC_1)
	{
		print_reg("+dec", reg, DEFAULT_W);
		uint16_t data = get_registers(g_registers, reg, BIT_16);
		set_registers(g_registers, reg, BIT_16, data - 1);

		g_flags.ZF = (data - 1) == 0;
		g_flags.OF = data && ((data - 1) & 0x8000) != (data & 0x8000); // 0 - 1 = -1 is a valid operation
		g_flags.OF = 0;
		g_flags.SF = ((data - 1) & 0x8000) == 0x8000;

		return 1;
	}
	else
		return 0;
}

