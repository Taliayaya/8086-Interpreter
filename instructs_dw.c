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
		struct calc_data data;
		struct print_data pdata;
		pdata = print_mrr("+add", byte2, d, w);
		data = process_operation(pdata, CALC_ADD, d, w);
		g_flags.SF = w ? (data.result & 0x8000) == 0x8000 : (data.result & 0x80) == 0x80;
		g_flags.ZF = data.result == 0;
		g_flags.CF = data.left > data.result;
		uint8_t res8;
		uint16_t res16;
		g_flags.OF = w ? __builtin_add_overflow(data.left, data.right, &res16) : __builtin_add_overflow((int8_t)data.left, (int8_t)data.right, &res8);
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
		struct print_data pdata;
		struct calc_data data;
		pdata = print_mrr("+sub", byte2, d, w);
		data = process_operation(pdata, CALC_SUB, d, w);
		g_flags.OF = w ? ((IS_NEG16(data.left) && !IS_NEG16(data.right) && !IS_NEG16(data.result)) || (!IS_NEG16(data.left) && IS_NEG16(data.right) && IS_NEG16(data.result))) : ((IS_NEG8(data.left) && !IS_NEG8(data.right) && !IS_NEG8(data.result)) || (!IS_NEG8(data.left) && IS_NEG8(data.right) && IS_NEG8(data.result)));
		g_flags.SF = w ? IS_NEG16(data.result) : IS_NEG8(data.result); // negative
		g_flags.ZF = data.result == 0; // zero
			//g_flags.PF // TODO
		g_flags.CF = data.left < data.right;

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
		struct print_data pdata;
		pdata = print_mrr("+cmp", byte2, d, w);
		uint16_t ldata, rdata, result;
		// MOD_REG == MOD_REG
		if (pdata.data_left.type == pdata.data_right.type) 	
		{
			ldata = get_registers(g_registers, pdata.data_left._reg, w);
			rdata = get_registers(g_registers, pdata.data_right._reg, w);
		}
		else // MOD_EA
		{
			if (d == 0) // mem, reg
			{
				rdata = get_registers(g_registers, 
					pdata.data_right._reg, w);
				ldata = get_memory(g_memory, 
					pdata.data_left._ea, w);
			}
			else // reg, mem
			{
				ldata = get_registers(g_registers, 
					pdata.data_left._reg, w);
				rdata = get_memory(g_memory, 
					pdata.data_right._ea, w);
			}
		}
		result = ldata - rdata;
		g_flags.CF = (uint16_t)ldata < (uint16_t)rdata;
		g_flags.SF = w ? (result & 0x8000) == 0x8000 : (result & 0x80) == 0x80;
		g_flags.ZF = result == 0;
		if (w)
			g_flags.OF = 
				((ldata & 0x8000) != 0x8000 && (rdata & 0x8000) == 0x8000 && g_flags.SF) ||
				(ldata & 0x8000) == 0x8000 && (rdata & 0x8000) != 0x8000 && !g_flags.SF;
		else
			g_flags.OF = 
				((ldata & 0x80) != 0x80 && (rdata & 0x80) == 0x80 && g_flags.SF) ||
				(ldata & 0x80) == 0x80 && (rdata & 0x80) != 0x80 && !g_flags.SF;



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
		struct print_data pdata;
		pdata = print_mrr("+and", byte2, d, w);

		struct calc_data data;
		data = process_operation(pdata, CALC_AND, d, w);
		g_flags.SF = w ? IS_NEG16(data.result) : IS_NEG8(data.result);
		g_flags.ZF = data.result == 0;
		g_flags.OF = 0;
		g_flags.CF = 0;


		
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
		struct calc_data data;
		data = process_operation(pdata, CALC_OR, d, w);
		// OC & CF cleared
		g_flags.OF = 0;
		g_flags.CF = 0;
		// SF & ZF & PF
		update_sf(&g_flags, data.result, w);
		update_zf(&g_flags, data.result);
		update_pf(&g_flags, data.result);

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
		struct print_data pdata;
		pdata = print_mrr("+xor", byte2, d, w);

		struct calc_data data = process_operation(pdata, CALC_XOR, d, w);

		g_flags.OF = 0; // cleared
		g_flags.CF = 0;

		g_flags.SF = w ? (data.result & 0x8000) == 0x8000 : (data.result & 0x80) == 0x80;
		g_flags.ZF = data.result == 0;

		return 1;
	}
	else
		return 0;
}




//int main()
//{
//	return 0;
//}
