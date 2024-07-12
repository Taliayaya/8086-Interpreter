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
		rdata = print_mr_data(OP_DONE_MARK"mov", byte2, w);

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
		sprintf(instr, "mov %s, [%04hx]", w ? "ax" : "al", addr);
		pretty_print(PC + 1, 2, instr);

		PC += 3;
		NOT_IMPLEMENTED("mov_3");
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
		sprintf(instr, "mov [%04hx], %s", addr, w ? "ax" : "al");
		pretty_print(PC + 1, 2, instr);

		PC += 3;

		NOT_IMPLEMENTED("mov_4");
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
			sprintf(instr, "add al, %hhx", byte2);
		}
		else
		{
			uint16_t data = byte2 | (g_text_segment[PC + 2] << 8);
			sprintf(instr, "add ax, %04hx", data);
		}
		pretty_print(PC + 1, 1 + w, instr);
		PC += 2 + w;

		NOT_IMPLEMENTED("add_2");
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
			sprintf(instr, "adc al, %hhx", byte2);
		}
		else
		{
			uint16_t data = byte2 | (g_text_segment[PC + 2] << 8);
			sprintf(instr, "adc ax, %04hx", data);
		}
		pretty_print(PC + 1, 1 + w, instr);
		PC += 2 + w;
		NOT_IMPLEMENTED("adc_2");
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
		struct operation_data rdata;
		rdata = print_mr(OP_DONE_MARK"inc", byte2, w);
		uint16_t data;
		if (rdata.type == MOD_REG)
		{
			data = get_registers(g_registers, rdata._reg, w);
			set_registers(g_registers, rdata._reg, w, data + 1);
		}
		else // MOD_EA
		{
			data = get_memory(g_memory, rdata._ea, w);
			set_memory(g_memory, rdata._ea, w, data + 1);
		}
		// do not touch CF
		// OF SF ZF AF
		g_flags.ZF = (data + 1) == 0;
		g_flags.OF = data == (w ? 0xFFFF : 0xFF);
		g_flags.SF = w ? ((data + 1) & 0x8000) == 0x8000 : ((data + 1) & 0x80 == 0x80);
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
		struct operation_data data;
		data = print_mr(OP_DONE_MARK"dec", byte2, w);

		uint16_t val;
		if (data.type == MOD_REG)
		{
			val = get_registers(g_registers, data._reg, w);
			set_registers(g_registers, data._reg, w, val - 1);
		}
		else // MOD_EA
		{
			val = get_memory(g_memory, data._ea, w);
			set_memory(g_memory, data._ea, w, val - 1);
		}

		g_flags.ZF = (val - 1) == 0;
		g_flags.OF = val && ((val - 1) & 0x8000) != (val & 0x8000); // 0 - 1 = -1 is a valid operation
		g_flags.OF = 0;
		g_flags.SF = ((val - 1) & 0x8000) == 0x8000;

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
		struct operation_data pdata;
		pdata = print_mr(OP_DONE_MARK"neg", byte2, w);
		uint16_t data;
		if (pdata.type == MOD_REG)
		{
			data = get_registers(g_registers, pdata._reg, w);
			set_registers(g_registers, pdata._reg, w, ~data + 1);
		}
		else // MOD_EA
		{
			data = get_memory(g_memory, pdata._ea, w);
			set_memory(g_memory, pdata._ea, w, ~data + 1);
		}

		g_flags.CF = data != 0;
		g_flags.ZF = (~data + 1) == 0;
		g_flags.OF = 0; // ?
		g_flags.SF = w ? ((~data + 1) & 0x8000) == 0x8000 : ((~data + 1) & 0x80) == 0x80;

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

		NOT_IMPLEMENTED("mul");
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

		NOT_IMPLEMENTED("imul");
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
		struct operation_data data;
		data = print_mr(OP_DONE_MARK"div", byte2, w);
		if (PROGRAM_MODE == DISSASSEMBLE)
			return -1;

		uint16_t up_nb, dwn_nb;
		up_nb = get_registers(g_registers, AX, BIT_16);
		if (data.type == MOD_REG)
			dwn_nb = get_registers(g_registers, data._reg, w);
		else
			dwn_nb = get_memory(g_memory, data._ea, w);
		if (dwn_nb == 0)
		{
			printf("div is 0, exit\n");
			exit(0);
		}

		if (w)
		{
			uint32_t up_nb = (g_registers[DX] << 16) | g_registers[AX];
			uint16_t q, r;
			q = up_nb / dwn_nb;
			r = up_nb % dwn_nb;
			g_registers[AX] = q;
			g_registers[DX] = r;
		}
		else
		{
			uint8_t q, r;
			q = up_nb / dwn_nb;
			r = up_nb % dwn_nb;
			// set quotient in lower byte of AL
			set_registers(g_registers, AL, BIT_8, q);
			// set remainder in higher byte of AH
			set_registers(g_registers, AH, BIT_8, r);

		}
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

		NOT_IMPLEMENTED("idiv");
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

		NOT_IMPLEMENTED("not");
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
		struct print_data pdata;
		pdata = print_mr_data(OP_DONE_MARK"and", byte2, w);
		uint16_t ldata, result;
		if (pdata.data_left.type == MOD_REG)
		{
			ldata = get_registers(g_registers, pdata.data_left._reg, w);
			result = ldata & pdata.data_right._imm16;	
			set_registers(g_registers, pdata.data_left._reg, w, result);
		}
		else // MOD_EA
		{
			ldata = get_memory(g_memory, pdata.data_left._ea, w);
			result = ldata & pdata.data_right._imm16;	
			set_memory(g_memory, pdata.data_left._ea, w, result);
		}

		g_flags.SF = w ? (result & 0x8000) == 0x8000 : (result & 0x80) == 0x80;
		g_flags.ZF = result == 0;
		g_flags.OF = 0;
		g_flags.CF = 0;

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
		struct print_data pdata;
		pdata = print_mrr(OP_DONE_MARK"xchg", byte2, 0, w);
		uint16_t ldata, rdata;
		if (pdata.data_left.type == pdata.data_right.type) 	
		{
			ldata = get_registers(g_registers, pdata.data_left._reg, w);
			rdata = get_registers(g_registers, pdata.data_right._reg, w);
			set_registers(g_registers, pdata.data_left._reg, w, rdata);
			set_registers(g_registers, pdata.data_right._reg, w, ldata);
		}
		else // MOD_EA
		{
			rdata = get_registers(g_registers, 
				pdata.data_right._reg, w);
			ldata = get_memory(g_memory, 
				pdata.data_left._ea, w);
			set_memory(g_memory, pdata.data_left._ea, w, rdata);
			set_registers(g_registers, pdata.data_right._reg, w, ldata);

		}
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

		struct print_data pdata;
		pdata = print_mrr(OP_DONE_MARK"test", byte2, 0, w);
		uint16_t ldata, rdata, result;
		if (pdata.data_left.type == MOD_REG)
			ldata = get_registers(g_registers, pdata.data_left._reg, w);
		else // MOD_EA
			ldata = get_memory(g_memory, pdata.data_left._ea, w);
		if (pdata.data_right.type == MOD_REG)
			rdata = get_registers(g_registers, pdata.data_right._reg, w);
		else // MOD_EA
			ldata = get_memory(g_memory, pdata.data_right._ea, w);
		
		result = ldata & rdata;	

		g_flags.SF = w ? (result & 0x8000) == 0x8000 : (result & 0x80) == 0x80;
		g_flags.ZF = result == 0;
		g_flags.OF = 0;
		g_flags.CF = 0;

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
		struct print_data pdata;
		pdata = print_mr_data(OP_DONE_MARK"test", byte2, w);
		uint16_t ldata, result;
		if (pdata.data_left.type == MOD_REG)
			ldata = get_registers(g_registers, pdata.data_left._reg, w);
		else // MOD_EA
			ldata = get_memory(g_memory, pdata.data_left._ea, w);
		result = ldata & pdata.data_right._imm16;	

		g_flags.SF = w ? (result & 0x8000) == 0x8000 : (result & 0x80) == 0x80;
		g_flags.ZF = result == 0;
		g_flags.OF = 0;
		g_flags.CF = 0;
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
		struct print_data pdata;
		pdata = print_mr_data(OP_DONE_MARK"or", byte2, w);
		uint16_t ldata, result;
		if (pdata.data_left.type == MOD_REG)
		{
			ldata = get_registers(g_registers, pdata.data_left._reg, w);
			result = ldata | pdata.data_right._imm16;	
			set_registers(g_registers, pdata.data_left._reg, w, result);
		}
		else // MOD_EA
		{
			ldata = get_memory(g_memory, pdata.data_left._ea, w);
			result = ldata | pdata.data_right._imm16;	
			set_memory(g_memory, pdata.data_left._ea, w, result);
		}

		g_flags.SF = w ? (result & 0x8000) == 0x8000 : (result & 0x80) == 0x80;
		g_flags.ZF = result == 0;
		g_flags.OF = 0;
		g_flags.CF = 0;

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
		struct print_data rdata;
		rdata = print_mr_data(OP_DONE_MARK"xor", byte2, w);
		uint16_t mem_reg, result;
		if (rdata.data_left.type == MOD_REG)
		{
			mem_reg = get_registers(g_registers, rdata.data_left._reg, w);
			result = mem_reg ^ rdata.data_right._imm16;
			set_registers(g_registers, rdata.data_left._reg, w, result);
		}
		else // MOD_EA
		{
			mem_reg = get_memory(g_memory, rdata.data_left._ea, w);
			result = mem_reg ^ rdata.data_right._imm16;
			set_memory(g_memory, rdata.data_left._ea, w, result);
		}

		g_flags.OF = 0; // cleared
		g_flags.CF = 0;

		g_flags.SF = w ? (result & 0x8000) == 0x8000 : (result & 0x80) == 0x80;
		g_flags.ZF = result == 0;
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
		uint16_t ldata, result;
		int8_t res8;
		if (w == 1)
		{
			data = byte2 | (g_text_segment[PC + 2] << 8);
			sprintf(instr, OP_DONE_MARK"cmp ax, %04hx", data);
		}
		else
		{
			sprintf(instr, OP_DONE_MARK"cmp al, %02hhx", byte2);
		}

		ldata = get_registers(g_registers, AX, w);
		result = ldata - data;
		g_flags.CF = (uint16_t)ldata < (uint16_t)data;
		g_flags.SF = w ? (result & 0x8000) == 0x8000 : (result & 0x80) == 0x80;
		g_flags.ZF = result == 0;
		if (w)
			g_flags.OF = 
				((ldata & 0x8000) != 0x8000 && (data & 0x8000) == 0x8000 && g_flags.SF) ||
				(ldata & 0x8000) == 0x8000 && (data & 0x8000) != 0x8000 && !g_flags.SF;
		else
			g_flags.OF = 
				((ldata & 0x80) != 0x80 && (data & 0x80) == 0x80 && g_flags.SF) ||
				(ldata & 0x80) == 0x80 && (data & 0x80) != 0x80 && !g_flags.SF;

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
		odata = print_mr(OP_DONE_MARK"push", byte2, BIT_16);
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
		odata = print_mr(OP_DONE_MARK"pop", byte2, BIT_16);
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
		struct operation_data data;
		data = print_mr(OP_DONE_MARK"call", byte2, 1);

		if (PROGRAM_MODE == DISSASSEMBLE)
			return 1;

		// push next instruction
		push_stack(PC, BIT_16);

		// go to call
		if (data.type == MOD_REG)
			PC = get_registers(g_registers, data._reg, BIT_16);
		else
			PC = get_memory(g_memory, data._ea, BIT_16);
			
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
		struct operation_data data;
		data = print_mr(OP_DONE_MARK"jmp", byte2, BIT_16);
		if (PROGRAM_MODE == DISSASSEMBLE)
			return 1;

		if (data.type == MOD_REG)
			PC = get_registers(g_registers, data._reg, BIT_16);
		else
			PC = get_memory(g_memory, data._ea, BIT_16);
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

		NOT_IMPLEMENTED("jmp_4");
		return 1;
	}
	else
		return 0;
}


