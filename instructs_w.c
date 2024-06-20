#include "instructs_w.h"


int op_w()
{
	static const W_Instruction instructions[] = {
		op_mov_1,
		op_mov_3,
		op_mov_4,
		op_add_2,
		op_adc_2,
		op_inc_0,
		op_dec_0,
		op_neg,
		op_mul,
		op_imul,
		op_div,
		op_idiv,
		op_not,
		op_and_1,
		op_xchg_0,
		op_test_0,
		op_test_1,
		op_or_1,
		op_xor_1,
		op_cmp_2
	};
	static const W_Instruction flags_instructions[] = {
		op_push_0,
		op_pop_0,
		op_call_1,
		op_jmp_2,
		op_jmp_4
	};
	static const size_t length = 
		sizeof(instructions) / sizeof(W_Instruction);
	static const size_t length_flag = 
		sizeof(flags_instructions) / sizeof(W_Instruction);

	uint8_t byte1, byte2, op, w, flag;
	byte1 = g_text_segment[PC];
	op = byte1 & W_MASK;
	byte2 = g_text_segment[PC + 1];
	w = W(byte1);
	flag = FLAG(byte2);

	size_t index = 0;
	while (index < length &&
		!instructions[index](op, flag, byte2, w))
		index++;
	if (index < length)
		return 1;
	
	index = 0;
	while (index < length_flag &&
		!flags_instructions[index](byte1, 
								   flag, byte2, 0)
		)
		index++;

	if (index < length_flag)
		return 1;

	return 0;
}

int op_mov_1(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_MOV_1 &&
		flag == OP_W_MOV_1_FLAG)
	{
		struct print_data rdata;
		PC += 2;
		rdata = print_mr_data("mov", byte2, w);

		if (rdata.data_left.type == MOD_REG)
		{
			set_registers(g_registers, rdata.data_left._reg, w,
				rdata.data_right._imm16);
		}
		else
		{
			set_memory(g_memory, rdata.data_left._ea, w,
				rdata.data_right._imm16);
		}
		return 1;
	}
	else
		return 0;
}

int op_mov_3(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_MOV_3)
	{
		uint16_t addr = byte2 | (g_text_segment[PC + 2] << 8);
		char instr[32];
		sprintf(instr, "mov %s, [%04hx]\n", w ? "ax" : "al", addr);
		pretty_print(PC + 1, 2, instr);

		PC += 3;
		return 1;
	}
	else
		return 0;
}

int op_mov_4(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_MOV_4)
	{
		uint16_t addr = byte2 | (g_text_segment[PC + 2] << 8);
		char instr[32];
		sprintf(instr, "mov [%04hx], %s\n", addr, w ? "ax" : "al");
		pretty_print(PC + 1, 2, instr);

		PC += 3;
		return 1;
	}
	else
		return 0;
}

int op_add_2(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_ADD_2)
	{
		char instr[32];
		if (w == 0)
		{
			sprintf(instr, "add al, %hhx\n", byte2);
		}
		else
		{
			uint16_t data = byte2 | (g_text_segment[PC + 2] << 8);
			sprintf(instr, "add ax, %04hx\n", data);
		}
		pretty_print(PC + 1, 1 + w, instr);
		PC += 2 + w;
		return 1;
	}
	else
		return 0;
}

int op_adc_2(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_ADC_2)
	{
		char instr[32];
		if (w == 0)
		{
			sprintf(instr, "adc al, %hhx\n", byte2);
		}
		else
		{
			uint16_t data = byte2 | (g_text_segment[PC + 2] << 8);
			sprintf(instr, "adc ax, %04hx\n", data);
		}
		pretty_print(PC + 1, 1 + w, instr);
		PC += 2 + w;
		return 1;
	}
	else
		return 0;
}


int op_inc_0(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_INC_0 && flag == OP_W_INC_0_FLAG)
	{
		PC += 2;
		print_mr("inc", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_dec_0(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_DEC_0 && flag == OP_W_DEC_0_FLAG)
	{
		PC += 2;
		print_mr("dec", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_neg(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_NEG && flag == OP_W_NEG_FLAG)
	{
		PC += 2;
		print_mr("neg", byte2, w);
		return 1;
	}
	else
		return 0;
}

// The following four op share the same op, but diff flag
int op_mul(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_MUL && flag == OP_W_MUL_FLAG)
	{
		PC += 2;
		print_mr("mul", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_imul(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_IMUL && flag == OP_W_IMUL_FLAG)
	{
		PC += 2;
		print_mr("imul", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_div(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_DIV && flag == OP_W_DIV_FLAG)
	{
		PC += 2;
		print_mr("div", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_idiv(
	uint8_t op, uint8_t flag, uint8_t byte2, uint8_t w)
{
	if (op == OP_W_IDIV && flag == OP_W_IDIV_FLAG)
	{
		PC += 2;
		print_mr("idiv", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_not(
	uint8_t op, uint8_t flag, uint8_t byte2, uint8_t w)
{
	if (op == OP_W_NOT && flag == OP_W_NOT_FLAG)
	{
		PC += 2;
		print_mr("not", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_and_1(
	uint8_t op, uint8_t flag, uint8_t byte2, uint8_t w)
{
	if (op == OP_W_AND_1 && flag == OP_W_AND_1_FLAG)
	{
		PC += 2;
		print_mr_data("and", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_xchg_0(
	uint8_t op, uint8_t flag, uint8_t byte2, uint8_t w)
{
	if (op == OP_XCHG_0)
	{
		PC += 2;
		print_mrr("xchg", byte2, 0, w);
		return 1;
	}
	else
		return 0;

}

int op_test_0(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_TEST_0)
	{
		PC += 2;
		print_mrr("test", byte2, 0, w);
		return 1;
	}
	else
		return 0;

}

int op_test_1(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_TEST_1 && flag == OP_W_TEST_1_FLAG)
	{
		PC += 2;
		print_mr_data("test", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_or_1(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_OR_1 && flag == OP_W_OR_1_FLAG)
	{
		PC += 2;
		print_mr_data("or", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_xor_1(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_XOR_1 && flag == OP_W_XOR_1_FLAG)
	{
		PC += 2;
		print_mr_data("xor", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_cmp_2(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_CMP_2)
	{
		uint16_t data;
		char instr[32];
		if (w == 1)
		{
			data = byte2 | (g_text_segment[PC + 2] << 8);
			sprintf(instr, "cmp ax, %04hx\n", data);
		}
		else
		{
			sprintf(instr, "cmp al, %02hhx\n", byte2);
		}
		pretty_print(PC + 1, 1 + w, instr);
		PC += 2 + w;
		return 1;
	}
	else
		return 0;
}



// FLAG ONLY (no w)

int op_push_0(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_PUSH_0 && flag == OP_PUSH_0_FLAG)
	{
		struct operation_data odata;
		PC += 2;
		odata = print_mr("+push", byte2, BIT_16);
		if (odata.type == MOD_REG)
			push_reg_stack(odata._reg, BIT_16);
		else
			push_mem_stack(odata._ea, BIT_16);
			
		return 1;
	}
	else
		return 0;
}

int op_pop_0(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_POP_0 && flag == OP_POP_0_FLAG)
	{
		struct operation_data odata;
		PC += 2;
		odata = print_mr("+pop", byte2, BIT_16);
		if (odata.type == MOD_REG)
			pop_reg_stack(odata._reg, BIT_16);
		else
			pop_mem_stack(odata._ea, BIT_16);
		return 1;
	}
	else
		return 0;
}

int op_call_1(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_CALL_1 && flag == OP_W_CALL_1_FLAG)
	{
		PC += 2;
		print_mr("call", byte2, 1);
		return 1;
	}
	else
		return 0;
}

int op_jmp_2(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_JMP_2 && flag == OP_JMP_2_FLAG)
	{
		PC += 2;
		print_mr("jmp", byte2, 1);
		return 1;
	}
	else
		return 0;
}

int op_jmp_4(uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_JMP_4 && flag == OP_JMP_4_FLAG)
	{
		PC += 2;
		print_mr("jmp", byte2, 1);
		return 1;
	}
	else
		return 0;
}


