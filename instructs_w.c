#include "instructs_w.h"


int op_w(uint8_t **text_segment)
{
	W_Instruction instructions[] = {
		op_mov_1,
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
	W_Instruction flags_instructions[] = {
		op_push_0,
		op_pop_0,
		op_call_1,
		op_jmp_2,
		op_jmp_4
	};
	size_t length = sizeof(instructions) / sizeof(W_Instruction);
	size_t length_flag = sizeof(flags_instructions) / sizeof(W_Instruction);

	uint8_t byte1, byte2, op, w, flag;
	byte1 = **text_segment;
	op = byte1 & W_MASK;
	byte2 = (*text_segment)[1];
	w = W(byte1);
	flag = FLAG(byte2);

	uint8_t *text_segment_copy = (*text_segment) + 2;

	size_t index = 0;
	while (index < length &&
		!instructions[index](&text_segment_copy, op, flag, byte2, w))
		index++;
	if (index < length)
	{
		*text_segment = text_segment_copy;
		return 1;
	}
	
	index = 0;
	while (index < length_flag &&
		!flags_instructions[index](&text_segment_copy, byte1, flag, byte2, 0))
		index++;

	if (index < length_flag)
	{
		(*text_segment) = text_segment_copy;
		return 1;
	}
	return 0;
}

int op_mov_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_MOV_1 &&
		flag == OP_W_MOV_1_FLAG)
	{
		print_mr_data(text_segment, "	mov", byte2, w);
		return 1;
	}
	else
		return 0;
}



int op_inc_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_INC_0 && flag == OP_W_INC_0_FLAG)
	{
		print_mr(text_segment, "	inc", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_dec_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_DEC_0 && flag == OP_W_DEC_0_FLAG)
	{
		print_mr(text_segment, "	dec", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_neg(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_NEG && flag == OP_W_NEG_FLAG)
	{
		print_mr(text_segment, "	neg", byte2, w);
		return 1;
	}
	else
		return 0;
}

// The following four op share the same op, but diff flag
int op_mul(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_MUL && flag == OP_W_MUL_FLAG)
	{
		print_mr(text_segment, "	mul", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_imul(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_IMUL && flag == OP_W_IMUL_FLAG)
	{
		print_mr(text_segment, "	imul", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_div(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_DIV && flag == OP_W_DIV_FLAG)
	{
		print_mr(text_segment, "	div", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_idiv(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_IDIV && flag == OP_W_IDIV_FLAG)
	{
		print_mr(text_segment, "	idiv", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_not(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_NOT && flag == OP_W_NOT_FLAG)
	{
		print_mr(text_segment, "	not", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_and_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_AND_1 && flag == OP_W_AND_1_FLAG)
	{
		print_mr_data(text_segment, "	and", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_xchg_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_XCHG_0)
	{
		print_mrr(text_segment, "	xchg", byte2, 0, w);
		return 1;
	}
	else
		return 0;

}

int op_test_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_TEST_0)
	{
		print_mrr(text_segment, "	test", byte2, 0, w);
		return 1;
	}
	else
		return 0;

}

int op_test_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_TEST_1 && flag == OP_W_TEST_1_FLAG)
	{
		print_mr_data(text_segment, "	test", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_or_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_OR_1 && flag == OP_W_OR_1_FLAG)
	{
		print_mr_data(text_segment, "	or", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_xor_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_XOR_1 && flag == OP_W_XOR_1_FLAG)
	{
		print_mr_data(text_segment, "	xor", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_cmp_2(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_CMP_2)
	{
		uint16_t data;
		printf("%02hhx", byte2);
		if (w == 1)
		{
			printf("%02hhx", **text_segment);
			data = byte2 | (**text_segment << 8);
			printf("		cmp ax, %04hx\n", data);
			(*text_segment) += 1;
		}
		else
		{
			printf("		cmp al, %02hhx\n", byte2);
		}
		return 1;
	}
	else
		return 0;
}



// FLAG ONLY (no w)

int op_push_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_PUSH_0 && flag == OP_PUSH_0_FLAG)
	{
		print_mr(text_segment, "	push", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_pop_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_POP_0 && flag == OP_POP_0_FLAG)
	{
		print_mr(text_segment, "	pop", byte2, w);
		return 1;
	}
	else
		return 0;
}

int op_call_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_W_CALL_1 && flag == OP_W_CALL_1_FLAG)
	{
		print_mr(text_segment, "	call", byte2, 1);
		return 1;
	}
	else
		return 0;
}

int op_jmp_2(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_JMP_2 && flag == OP_JMP_2_FLAG)
	{
		print_mr(text_segment, "	jmp", byte2, 1);
		return 1;
	}
	else
		return 0;
}

int op_jmp_4(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w)
{
	if (op == OP_JMP_4 && flag == OP_JMP_4_FLAG)
	{
		print_mr(text_segment, "	jmp", byte2, 1);
		return 1;
	}
	else
		return 0;
}


