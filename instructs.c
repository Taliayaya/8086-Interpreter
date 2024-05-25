#include "instructs.h"

typedef int (*Instruction)(uint8_t **, uint8_t);
typedef int (*PC_Instruction)(uint8_t **, uint8_t, uint16_t);


int op_others(uint8_t **text_segment, uint16_t pc)
{
	Instruction instructions[] = {
		op_push_2,
		op_pop_2,
		op_in,
		op_out,
		op_xlat,
		op_lea,
		op_lds,
		op_les,
		op_lahf,
		op_sahf,
		op_pushf,
		op_popf,
		op_mov_2,
		op_int,
		op_ret
	};
	size_t length = sizeof(instructions) / sizeof(Instruction);

	uint8_t byte1 = **text_segment;
	uint8_t *text_segment_copy = (*text_segment) + 1;

	size_t index = 0;
	while (index < length 
		&& !instructions[index](&text_segment_copy, byte1))
		index++;

	if (index < length)
	{
		(*text_segment) = text_segment_copy;
		return 1;
	}
	return 0;
}

int op_pc(uint8_t **text_segment, uint16_t pc)
{
	PC_Instruction instructions[] = {
		op_call_0,
		op_jmp,
		op_cond_jmp
	};
	size_t length = sizeof(instructions) / sizeof(PC_Instruction);

	uint8_t byte1 = **text_segment;
	uint8_t *text_segment_copy = (*text_segment) + 1;


	size_t index = 0;
	while (index < length 
		&& !instructions[index](&text_segment_copy, byte1, pc))
		index++;

	if (index < length)
	{
		(*text_segment) = text_segment_copy;
		return 1;
	}
	return 0;

}

// PUSH
int op_push_2(uint8_t **text_segment, uint8_t byte1)
{
	if (IS_OP(byte1, OP_PUSH_2_MASK, OP_PUSH_2))
	{
		uint8_t reg = byte1 & 0b00011000;
		printf("		push %s\n", get_segreg(reg));
		return 1;
	}
	else
		return 0;
}
// POP
int op_pop_2(uint8_t **text_segment, uint8_t byte1)
{
	if (IS_OP(byte1, OP_POP_2_MASK, OP_POP_2))
	{
		uint8_t reg = byte1 & 0b00011000;
		printf("		pop %s\n", get_segreg(reg));
		return 1;
	}
	else
		return 0;
}


// IN = Input from
int op_in(uint8_t **text_segment, uint8_t byte1)
{
	uint8_t w = W(byte1);
	if (IS_OP(byte1, OP_IN_0_MASK, OP_IN_0))
	{
		uint8_t port = **text_segment;
		printf("%02hhx		in %s, %02hhx\n", port, w ? "ax" : "al", port);
		(*text_segment) += 1;
	}
	else if (IS_OP(byte1, OP_IN_1_MASK, OP_IN_1))
	{
		printf("		in %s, dx\n", w ? "ax" : "al");
	}
	else
		return 0;
	return 1;
}

int op_out(uint8_t **text_segment, uint8_t byte1)
{
	uint8_t w = W(byte1);
	if (IS_OP(byte1, OP_OUT_0_MASK, OP_OUT_0))
	{
		uint8_t port = **text_segment;
		printf("%02hhx		out %s, %02hhx\n", port, 
			w ? "ax" : "al", port);
		(*text_segment) += 1;
	}
	else if (IS_OP(byte1, OP_OUT_1_MASK, OP_OUT_1))
	{
		printf("		out %s, dx\n", w ? "ax" : "al");
	}
	else
		return 0;
	return 1;
}


// LOAD & FLAGS STUFF
int op_xlat(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_XLAT)
	{
		printf("		xlat\n");
		return 1;
	}
	else
		return 0;
}

int op_lea(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_LEA)
	{
		uint8_t byte2 = **text_segment;
		(*text_segment) += 1;
		print_mrr(text_segment, "	lea", byte2, 1, 1);
		return 1;
	}
	else
		return 0;
}

int op_lds(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_LDS)
	{
		uint8_t byte2 = **text_segment;
		(*text_segment) += 1;
		print_mrr(text_segment, "	lds", byte2, 0, 0);
		return 1;
	}
	else
		return 0;
}

int op_les(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_LES)
	{
		uint8_t byte2 = **text_segment;
		(*text_segment) += 1;
		print_mrr(text_segment, "	les", byte2, 0, 0);
		return 1;
	}
	else
		return 0;
}

int op_lahf(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_LAHF)
	{
		printf("		lahf\n");
		return 1;
	}
	else
		return 0;
}

int op_sahf(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_SAHF)
	{
		printf("		sahf\n");
		return 1;
	}
	else
		return 0;
}

int op_pushf(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_PUSHF)
	{
		printf("		pushf\n");
		return 1;
	}
	else
		return 0;
}

int op_popf(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_POPF)
	{
		printf("		popf\n");
		return 1;
	}
	else
		return 0;
}

// MOV
int op_mov_2(uint8_t **text_segment, uint8_t byte1)
{
	if (IS_OP(byte1, OP_MOV_2_MASK, OP_MOV_2))
	{
		uint8_t w, reg;
		w = (byte1 & 0b00001000) >> 3;
		reg = R_M(byte1);

		printf("%02hhx", text_segment[0][0]);

		uint16_t data = text_segment[0][0];
		if (w == 1)
		{
			data |= (text_segment[0][1] << 8);
			printf("%02hhx", text_segment[0][1]);
			(*text_segment) += 2;
		}
		else
			(*text_segment) += 1;
		printf("		mov %s, %04hx\n", get_reg(reg, w), data);
		return 1;
	}
	else
		return 0;
}


// CALL
int op_call_0(uint8_t **text_segment, uint8_t op, uint16_t pc)
{
	if (op == OP_CALL_0)
	{
		int16_t increment = (text_segment[0][1] << 8) | 
			text_segment[0][0];
		printf("%02hhx%02hhx	 	call %04hx\n",  
			text_segment[0][0], text_segment[0][1], 
			pc + increment + 3);
		(*text_segment) += 2;

		return 1;	
	}
	else
		return 0;
}

// JUMPS
int op_jmp(uint8_t **text_segment, uint8_t op, uint16_t pc)
{
	if (op == OP_JMP_0)
	{
		uint8_t byte1, byte2;
		byte1 = text_segment[0][0];
		byte2 = text_segment[0][1];
		int16_t data16b = (byte2 << 8) | byte1;		
		int16_t target = pc + 3 + data16b;
		printf("%02hhx%02hhx		jmp %04hx\n",
			byte1, byte2, target);
		(*text_segment) += 2;
	}
	else if (op == OP_JMP_1)
	{
		int8_t disp = text_segment[0][0];
		printf("%02hhx		jmp short %04hx\n",
			disp, pc + 2 + disp);
		(*text_segment) += 1;
	}
	else
		return 0;
	return 1;
}

// COND JUMPS
int op_cond_jmp(uint8_t **text_segment, uint8_t op, uint16_t pc)
{
	int8_t disp = text_segment[0][0];
	printf("%02hhx", disp);
	char *op_name;
	
	switch(op)
	{
		case OP_JE:
			op_name = "je";
			break;
		case OP_JNE:
			op_name = "jne";
			break;
		case OP_JNL:
			op_name = "jnl";
			break;
		default:
			return 0;
	}
	
	printf("		%s %04hx\n", op_name, pc + 2 + disp);
	(*text_segment) += 1;
	
	return 1;
}

// INT
int op_int(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_INT_0)
	{
		uint8_t type = text_segment[0][0];
		printf("%02hhx		int %hhx\n", type, type);
		(*text_segment) += 1;
	}
	else if (op == OP_INT_1)
	{
		printf("		int 3\n");
	}
	else
		return 0;
	return 1;
}

int op_ret(uint8_t **text_segment, uint8_t op)
{
	if (op == OP_RET_0)
	{
		printf("		ret\n");
	}
	else
		return 0;
	return 1;
}

