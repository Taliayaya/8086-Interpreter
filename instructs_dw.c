#include "instructs_dw.h"
#include "utils.h"

int op_dw(uint8_t **text_segment)
{
	DW_Instruction instructions[] = {
		
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
	size_t length = sizeof(instructions) / sizeof(DW_Instruction);

	uint8_t byte1, byte2, op, d, w;
	byte1 = **text_segment;
	op = byte1 & DW_MASK;
	byte2 = (*text_segment)[1];
	d = D(byte1);
	w = W(byte1);

	uint8_t *text_segment_copy = (*text_segment) + 2;


	size_t index = 0;
	while (index < length &&
		!instructions[index](&text_segment_copy, op, byte2, d, w))
		index++;

	if (index < length)
	{
		(*text_segment) = text_segment_copy;
		return 1;
	}
	return 0;
}

int op_mov_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_MOV_0)
	{
		struct mrr_data rdata;
		rdata = print_mrr(text_segment, "	mov", byte2, d, w);
		if (rdata.mdata.type == MOD_REG)
		{
			int16_t data = get_registers(g_registers, rdata.reg_from, w);
			set_registers(g_registers, rdata.reg_to, w, data);
		}
		else // EA
		{
			if (d)
			{
				int16_t data = get_registers(g_registers, rdata.reg_from, w);
				set_memory(g_memory, rdata.mdata.ea, data, w);
			}
			else
			{
				int16_t data = get_memory(g_memory, rdata.mdata.ea, w);
				set_registers(g_registers, rdata.reg_to, data, w);
			}
		}
		return 1;
	}
	else
		return 0;
}

int op_add_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_ADD_0)
	{
		print_mrr(text_segment, "	add", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_adc_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_ADC_0)
	{
		print_mrr(text_segment, "	adc", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_sub_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_SUB_0)
	{
		print_mrr(text_segment, "	sub", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_ssb_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_SSB_0)
	{
		print_mrr(text_segment, "	sbb", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_cmp_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_CMP_0)
	{
		print_mrr(text_segment, "	cmp", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_and_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_AND_0)
	{
		print_mrr(text_segment, "	and", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_or_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_OR_0)
	{
		print_mrr(text_segment, "	or", byte2, d, w);
		return 1;
	}
	else
		return 0;
}

int op_xor_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w)
{
	if (op == OP_DW_XOR_0)
	{
		print_mrr(text_segment, "	xor", byte2, d, w);
		return 1;
	}
	else
		return 0;
}




//int main()
//{
//	return 0;
//}
