#include "instructs_sw.h"

#define DIGIT_FORMAT 1
#define HEXA_FORMAT 0

int op_sw()
{
	static SW_Instruction instructions[] = {
		op_add_1,
		op_adc_1,
		op_sub_1,
		op_ssb_1,
		op_cmp_1,
		
	};
	static size_t length = sizeof(instructions) / sizeof(SW_Instruction);

	uint8_t byte1, byte2, op, flag, s, w;
	byte1 = g_text_segment[PC];
	op = byte1 & SW_MASK;
	byte2 = g_text_segment[PC + 1];
	flag = FLAG(byte2);
	s = D(byte1);
	w = W(byte1);

	size_t index = 0;
	while (index < length &&
		!instructions[index](op, flag, byte2, s, w))
		index++;

	return index < length;
}

int op_add_1(uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_ADD_1 && flag == OP_SW_ADD_1_FLAG)
	{
		PC += 2;
		struct print_data rdata = print_mr_sw("+add", byte2, s, w, HEXA_FORMAT);
		int16_t result;
		int16_t immediate = w ? rdata.data_right._imm16 : rdata.data_right._imm8;
		if (rdata.data_left.type == MOD_REG)
		{
			int16_t reg_data = get_registers(g_registers, 
				rdata.data_left._reg, w);
			// should take w into account?
			g_flags.OF = __builtin_add_overflow(reg_data, immediate, &result); // signed carry
			g_flags.AF = ((reg_data & 0x0F) < (immediate & 0x0F)); // nibble overflow
			g_flags.CF = (uint16_t)result < (uint16_t)reg_data || (uint16_t)result < (uint16_t)immediate;


			set_registers(g_registers, rdata.data_left._reg, w, result);

		}
		else
		{
			int16_t mem_data = get_memory(g_memory, rdata.data_left._ea, w);
			result =  mem_data + rdata.data_right._imm16;
			set_memory(g_memory, rdata.data_left._ea, w, result);
			int8_t res8;

			g_flags.OF = __builtin_add_overflow(mem_data, rdata.data_right._imm16, &res8); // signed carry
			g_flags.SF = result < 0; // negative
			g_flags.ZF = result == 0; // zero
			g_flags.AF = ((mem_data & 0x0F) < (rdata.data_right._imm16 & 0x0F)); // nibble overflow
			g_flags.CF = (uint16_t)result < (uint16_t)mem_data || (uint16_t)result < (uint16_t)immediate;
		}
		update_zf(&g_flags, result);
		update_pf(&g_flags, result);
		update_sf(&g_flags, result, w);


		return 1;
	}
	else
		return 0;
}

int op_adc_1(uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_ADC_1 && flag == OP_SW_ADC_1_FLAG)
	{
		PC += 2;
		print_mr_sw("adc", byte2, s, w, HEXA_FORMAT);
		return 1;
	}
	else
		return 0;
}

int op_sub_1(uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_SUB_1 && flag == OP_SW_SUB_1_FLAG)
	{
		struct print_data rdata;
		PC += 2;
		rdata = print_mr_sw("+sub", byte2, s, w, HEXA_FORMAT);
		int16_t result;
		int16_t immediate = w ? rdata.data_right._imm16 : rdata.data_right._imm8;
		if (rdata.data_left.type == MOD_REG)
		{
			int16_t reg_data = get_registers(g_registers, 
				rdata.data_left._reg, w);
			// should take w into account?
			g_flags.OF = __builtin_sub_overflow(reg_data, immediate, &result); // signed carry
			g_flags.AF = ((reg_data & 0x0F) < (immediate & 0x0F)); // nibble overflow
			g_flags.CF = (uint16_t)reg_data < (uint16_t)immediate;


			set_registers(g_registers, rdata.data_left._reg, w, result);

		}
		else
		{
			int16_t mem_data = get_memory(g_memory, rdata.data_left._ea, w);
			result =  mem_data - rdata.data_right._imm16;
			set_memory(g_memory, rdata.data_left._ea, w, result);
			int8_t res8;

			g_flags.OF = __builtin_sub_overflow(mem_data, rdata.data_right._imm16, &res8); // signed carry
			g_flags.SF = result < 0; // negative
			g_flags.ZF = result == 0; // zero
			g_flags.AF = ((mem_data & 0x0F) < (rdata.data_right._imm16 & 0x0F)); // nibble overflow
			//g_flags.PF // TODO
			g_flags.CF = mem_data < rdata.data_right._imm16;

		}
		update_zf(&g_flags, result);
		update_pf(&g_flags, result);
		update_sf(&g_flags, result, w);


		return 1;
	}
	else
		return 0;
}

int op_ssb_1(uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_SSB_1 && flag == OP_SW_SSB_1_FLAG)
	{
		PC += 2;
		print_mr_sw("sbb", byte2, s, w, HEXA_FORMAT);
		return 1;
	}
	else
		return 0;
}

int op_cmp_1(uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_CMP_1 && flag == OP_SW_CMP_1_FLAG)
	{
		PC += 2;
		struct print_data data;
		data = print_mr_sw("+cmp", byte2, s, w, DIGIT_FORMAT);

		uint16_t ldata;
		uint16_t rdata = data.data_right._imm16;
		if (s == 1 && w == 1) // sign extend
			rdata = IS_NEG8(rdata) ? 0XFF00 | rdata : rdata;
	
		int16_t result, res16;
		int8_t res8;

		if (data.data_left.type == MOD_REG)
		{
			ldata = get_registers(g_registers, 
				data.data_left._reg, w);
		}
		else // MOD_EA
		{
			ldata = get_memory(g_memory, data.data_left._ea, w);
		}
		result = ldata - rdata;

		update_zf(&g_flags, result);
		update_pf(&g_flags, result);
		update_sf(&g_flags, result, w);

		g_flags.OF = w ? ((IS_NEG16(ldata) && !IS_NEG16(rdata) && !IS_NEG16(result)) || (!IS_NEG16(ldata) && IS_NEG16(rdata) && IS_NEG16(result))) : ((IS_NEG8(ldata) && !IS_NEG8(rdata) && !IS_NEG8(result)) || (!IS_NEG8(ldata) && IS_NEG8(rdata) && IS_NEG8(result)));
		g_flags.CF = (uint16_t)ldata < (uint16_t)rdata;
		return 1;
	}
	else
		return 0;
}
