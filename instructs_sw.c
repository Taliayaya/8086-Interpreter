#include "instructs_sw.h"

int op_sw(uint8_t **text_segment)
{
	SW_Instruction instructions[] = {
		op_add_1,
		op_adc_1,
		op_sub_1,
		op_ssb_1,
		op_cmp_1,
		
	};
	size_t length = sizeof(instructions) / sizeof(SW_Instruction);

	uint8_t byte1, byte2, op, flag, s, w;
	byte1 = **text_segment;
	op = byte1 & SW_MASK;
	byte2 = (*text_segment)[1];
	flag = FLAG(byte2);
	s = D(byte1);
	w = W(byte1);

	uint8_t *text_segment_copy = *text_segment + 2;

	size_t index = 0;
	while (index < length &&
		!instructions[index](&text_segment_copy, op, flag, byte2, s, w))
		index++;

	if (index < length)
	{
		(*text_segment) = text_segment_copy;
		return 1;
	}
	return 0;
}

int op_add_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_ADD_1 && flag == OP_SW_ADD_1_FLAG)
	{
		print_mr_sw(text_segment, "	add", byte2, s, w);
		return 1;
	}
	else
		return 0;
}

int op_adc_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_ADC_1 && flag == OP_SW_ADC_1_FLAG)
	{
		print_mr_sw(text_segment, "	adc", byte2, s, w);
		return 1;
	}
	else
		return 0;
}

int op_sub_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_SUB_1 && flag == OP_SW_SUB_1_FLAG)
	{
		print_mr_sw(text_segment, "	sub", byte2, s, w);
		return 1;
	}
	else
		return 0;
}

int op_ssb_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_SSB_1 && flag == OP_SW_SSB_1_FLAG)
	{
		print_mr_sw(text_segment, "	ssb", byte2, s, w);
		return 1;
	}
	else
		return 0;
}

int op_cmp_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w)
{
	if (op == OP_SW_CMP_1 && flag == OP_SW_CMP_1_FLAG)
	{
		print_mr_sw(text_segment, "	cmp", byte2, s, w);
		return 1;
	}
	else
		return 0;
}
