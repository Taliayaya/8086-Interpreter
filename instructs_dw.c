#include "instructs_dw.h"
#include "utils.h"

int op_dw()
{
	static DW_Instruction instructions[] = {
		
		op_mov_0,
		op_add_0,
		op_adc_0,
		op_sub_0,
		op_ssb_0,
		op_cmp_0,
		op_and_0,
		op_or_0,
		op_xor_0,
	};
	static size_t length = sizeof(instructions) / sizeof(DW_Instruction);

	uint8_t byte1, byte2, op, d, w;
	byte1 = g_text_segment[PC];
	op = byte1 & DW_MASK;
	byte2 = g_text_segment[PC + 1];
	d = D(byte1);
	w = W(byte1);


	size_t index = 0;
	while (index < length &&
		!instructions[index](op, byte2, d, w))
		index++;

	return index < length;
}

int op_mov_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_MOV_0)
	{
		struct print_data rdata;
		PC += 2;
		rdata = print_mrr("+mov", byte2, d, w);
		if (rdata.data_left.type == MOD_REG && rdata.data_right.type == MOD_REG)
		{
			int16_t data = get_registers(g_registers, rdata.data_right._reg, w);
			set_registers(g_registers, rdata.data_left._reg, w, data);
		}
		else // EA
		{
			if (d == 0)
			{
				int16_t data = get_registers(g_registers, rdata.data_right._reg, w);
				set_memory(g_memory, rdata.data_left._ea, w, data);
			}
			else
			{
				int16_t data = get_memory(g_memory, rdata.data_right._ea, w);
				set_registers(g_registers, rdata.data_left._reg, w, data);
				
			}
		}
		return 1;
	}
	else
		return 0;
}

int op_add_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_ADD_0)
	{
		PC += 2;
		print_mrr("add", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_adc_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_ADC_0)
	{
		PC += 2;
		print_mrr("adc", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_sub_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_SUB_0)
	{
		PC += 2;
		print_mrr("sub", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_ssb_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_SSB_0)
	{
		PC += 2;
		print_mrr("sbb", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_cmp_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_CMP_0)
	{
		PC += 2;
		print_mrr("cmp", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_and_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_AND_0)
	{
		PC += 2;
		print_mrr("and", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_or_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_OR_0)
	{
		PC += 2;
		struct print_data pdata;
		pdata = print_mrr("+or", byte2, d, w);
		// REG == REG
		int16_t result;
		if (pdata.data_left.type == pdata.data_right.type)
		{
			uint16_t lreg = get_registers(g_registers, 
				pdata.data_left._reg, w);
			uint16_t rreg = get_registers(g_registers, 
				pdata.data_right._reg, w);
			result = lreg | rreg;
			set_registers(g_registers, pdata.data_left._reg, w, result);
		}
		else // EA
		{
			if (d == 0)
			{
				uint16_t rreg = get_registers(g_registers, 
					pdata.data_right._reg, w);
				uint16_t mem = get_memory(g_memory, 
					pdata.data_left._ea, w);
				result = rreg | mem;
				set_memory(g_memory, pdata.data_left._ea, w, result);
			}
			else
			{
				uint16_t lreg = get_registers(g_registers, 
					pdata.data_left._reg, w);
				uint16_t mem = get_memory(g_memory, 
					pdata.data_right._ea, w);
				result = lreg | mem;
				set_registers(g_registers, pdata.data_left._reg, w,
					result);
			}
		}
		// OC & CF cleared
		g_flags.OF = 0;
		g_flags.CF = 0;
		// SF & ZF & PF
		update_sf(&g_flags, result);
		update_zf(&g_flags, result);
		update_pf(&g_flags, result);

		return 1;
	}
	else
		return 0;
}

int op_xor_0(uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_XOR_0)
	{
		PC += 2;
		print_mrr("xor", byte2, d, w);
		return 1;
	}
	else
		return 0;
}




//int main()
//{
//	return 0;
//}
