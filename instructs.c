#include "instructs.h"

typedef int (*Instruction)(uint8_t);
typedef int (*PC_Instruction)(uint8_t);


int op_others()
{
	static const Instruction instructions[] = {
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
		op_ret,
		op_cbw,
		op_cwd,
		op_ssb_2,
		op_sub_2,
		op_logic,
		op_clc,
		op_cmc,
		op_stc,
		op_cld,
		op_std,
		op_cli,
		op_sti,
		op_hlt,
		op_wait,
		op_test_2,
		op_rep,
		op_string

	};
	static const size_t length = sizeof(instructions) / sizeof(Instruction);

	uint8_t byte1 = g_text_segment[PC];

	size_t index = 0;
	while (index < length 
		&& !instructions[index](byte1))
		index++;

	if (index < length)
		return 1;
	return 0;
}

int op_pc()
{
	static const PC_Instruction instructions[] = {
		op_call_0,
		op_jmp,
		op_cond_jmp
	};
	static const size_t length = sizeof(instructions) / sizeof(PC_Instruction);

	uint8_t byte1 = g_text_segment[PC];

	size_t index = 0;
	while (index < length 
		&& !instructions[index](byte1))
		index++;

	if (index < length)
		return 1;
	return 0;

}

// PUSH
int op_push_2(uint8_t byte1)
{
	if (IS_OP(byte1, OP_PUSH_2_MASK, OP_PUSH_2))
	{
		uint8_t reg = byte1 & 0b00011000;
		char instr[32];
		sprintf(instr, "push %s\n", get_segreg(reg));
		pretty_print(PC + 1, 1, instr);
		PC += 1;
		return 1;
	}
	else
		return 0;
}
// POP
int op_pop_2(uint8_t byte1)
{
	if (IS_OP(byte1, OP_POP_2_MASK, OP_POP_2))
	{
		char instr[32];
		uint8_t reg = byte1 & 0b00011000;
		PC += 1;
		sprintf(instr, "pop %s\n", get_segreg(reg));
		pretty_print(PC + 1, 1, instr);

		return 1;
	}
	else
		return 0;
}


// IN = Input from
int op_in(uint8_t byte1)
{
	uint8_t w = W(byte1);
	char instr[32];
	if (IS_OP(byte1, OP_IN_0_MASK, OP_IN_0))
	{
		uint8_t port = g_text_segment[PC + 1];
		sprintf(instr, "in %s, %02hhx\n", w ? "ax" : "al", port);
		pretty_print(PC + 1, 1, instr);
		PC += 2;
	}
	else if (IS_OP(byte1, OP_IN_1_MASK, OP_IN_1))
	{
		sprintf(instr, "in %s, dx\n", w ? "ax" : "al");
		pretty_print(PC + 1, 0, instr);
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_out(uint8_t byte1)
{
	uint8_t w = W(byte1);
	char instr[32];
	if (IS_OP(byte1, OP_OUT_0_MASK, OP_OUT_0))
	{
		uint8_t port = g_text_segment[PC + 1];
		sprintf(instr, "out %s, %02hhx\n", w ? "ax" : "al", port);
		pretty_print(PC + 1, 1, instr);

		PC += 2;
	}
	else if (IS_OP(byte1, OP_OUT_1_MASK, OP_OUT_1))
	{
		sprintf(instr, "out %s, dx\n", w ? "ax" : "al");
		pretty_print(PC + 1, 0, instr);
		PC += 1;
	}
	else
		return 0;
	return 1;
}


// LOAD & FLAGS STUFF
int op_xlat(uint8_t op)
{
	if (op == OP_XLAT)
	{
		pretty_print(PC + 1, 0, "xlat\n");

		PC += 1;
		return 1;
	}
	else
		return 0;
}

int op_lea(uint8_t op)
{
	if (op == OP_LEA)
	{
		uint8_t byte2 = g_text_segment[PC + 1];
		PC += 2;
		struct print_data rdata;
		rdata = print_mrr("+lea", byte2, 1, 1);
		if (rdata.data_right.type == MOD_EA)
			set_registers(g_registers, rdata.data_left._reg, 
				BIT_16, rdata.data_right._ea);
		else 
			printf("UNDEFINED\n");

		return 1;
	}
	else
		return 0;
}

int op_lds(uint8_t op)
{
	if (op == OP_LDS)
	{
		uint8_t byte2 = g_text_segment[PC + 1];
		PC += 2;
		print_mrr("lds", byte2, 0, 0);
		return 1;
	}
	else
		return 0;
}

int op_les(uint8_t op)
{
	if (op == OP_LES)
	{
		uint8_t byte2 = g_text_segment[PC + 1];
		PC += 2;
		print_mrr("les", byte2, 0, 0);
		return 1;
	}
	else
		return 0;
}

int op_lahf(uint8_t op)
{
	if (op == OP_LAHF)
	{
		pretty_print(PC + 1, 0, "lahf\n");
		PC += 1;
		return 1;
	}
	else
		return 0;
}

int op_sahf(uint8_t op)
{
	if (op == OP_SAHF)
	{
		pretty_print(PC + 1, 0, "sahf\n");
		PC += 1;
		return 1;
	}
	else
		return 0;
}

int op_pushf(uint8_t op)
{
	if (op == OP_PUSHF)
	{
		pretty_print(PC + 1, 0, "pushf\n");
		PC += 1;
		return 1;
	}
	else
		return 0;
}

int op_popf(uint8_t op)
{
	if (op == OP_POPF)
	{
		pretty_print(PC + 1, 0, "popf\n");
		PC += 1;
		return 1;
	}
	else
		return 0;
}

// MOV
int op_mov_2(uint8_t byte1)
{
	if (IS_OP(byte1, OP_MOV_2_MASK, OP_MOV_2))
	{
		uint8_t w, reg;
		w = (byte1 & 0b00001000) >> 3;
		reg = R_M(byte1);

		uint16_t data = g_text_segment[PC + 1];
		if (w == 1)
		{
			data |= (g_text_segment[PC + 2] << 8);
			PC += 3;
		}
		else
			PC += 2;
		set_registers(g_registers, reg, w, data);

		char instr[32];
		sprintf(instr, "+mov %s, %04hx\n", get_reg(reg, w), data);
		pretty_print(PC + 1, 2, instr);
		return 1;
	}
	else
		return 0;
}


// CALL
int op_call_0(uint8_t op)
{
	if (op == OP_CALL_0)
	{
		int16_t increment = (g_text_segment[PC + 2] << 8) | 
			g_text_segment[PC + 1];

		push_stack(PC + 3, BIT_16); // Push next instruction

		char instr[32];
		sprintf(instr, "+call %04hx\n", PC + 3 + increment);
		pretty_print(PC + 1, 2, instr);

		PC += increment + 3; // go to function

		return 1;	
	}
	else
		return 0;
}

// JUMPS
int op_jmp(uint8_t op)
{
	if (op == OP_JMP_0)
	{
		uint8_t byte1, byte2;
		byte1 = g_text_segment[PC + 1];
		byte2 = g_text_segment[PC + 2];
		int16_t data16b = (byte2 << 8) | byte1;		
		uint16_t target = PC + 3 + data16b;

		char instr[32];
		sprintf(instr, "+jmp %04hx\n", target);
		pretty_print(PC + 1, 2, instr);
		PC = target;
	}
	else if (op == OP_JMP_1)
	{
		int8_t disp = g_text_segment[PC + 1];
		char instr[32];
		sprintf(instr, "+jmp short %04hx\n", PC + 2 + disp);
		pretty_print(PC + 1, 1, instr);

		PC += 2 + disp;
	}
	else
		return 0;
	return 1;
}

// COND JUMPS
int op_cond_jmp(uint8_t op)
{
	int8_t disp = g_text_segment[PC + 1];
	char *op_name;

	uint8_t can_jump = 0;
	
	switch(op)
	{
		case OP_JE:
			op_name = "+je";
			can_jump = g_flags.ZF;
			break;
		case OP_JL:
			op_name = "+jl";
			can_jump = g_flags.SF != g_flags.OF;
			break;
		case OP_JLE:
			op_name = "+jle";
			can_jump = g_flags.ZF || (g_flags.SF != g_flags.OF);
			break;
		case OP_JB:
			op_name = "+jb";
			can_jump = g_flags.CF;
			break;
		case OP_JBE:
			op_name = "+jbe";
			can_jump = g_flags.CF || g_flags.ZF;
			break;
		case OP_JP:
			op_name = "jl";
			break;
		case OP_JO:
			op_name = "jo";
			break;
		case OP_JS:
			op_name = "js";
			break;
		case OP_JNE:
			op_name = "+jne";
			can_jump = !g_flags.ZF;
			break;
		case OP_JNL:
			op_name = "+jnl";
			can_jump = g_flags.SF == g_flags.OF;
			break;
		case OP_JNLE:
			op_name = "+jnle";
			can_jump = !g_flags.ZF && (g_flags.OF == g_flags.SF);
			break;
		case OP_JNB:
			op_name = "+jnb";
			can_jump = !g_flags.CF;
			break;
		case OP_JNBE:
			op_name = "+jnbe";
			can_jump = !g_flags.CF && !g_flags.ZF;
			break;
		case OP_JNP:
			op_name = "jnp";
			break;
		case OP_JNO:
			op_name = "jno";
			break;
		case OP_JNS:
			op_name = "jns";
			break;
		case OP_LOOP:
			op_name = "loop";
			break;
		case OP_LOOPZ:
			op_name = "loopz";
			break;
		case OP_LOOPNZ:
			op_name = "loopnz";
			break;
		case OP_JCXZ:
			op_name = "jcxz";
			break;

		default:
			return 0;
	}
	
	char instr[32];
	sprintf(instr, "%s %04hx\n", op_name, PC + 2 + disp);
	pretty_print(PC + 1, 1, instr);

	PC += 2;
	if (can_jump)
		PC += disp;
	
	return 1;
}

// INT
int op_int(uint8_t op)
{
	if (op == OP_INT_0)
	{
		char instr[32];
		uint8_t type = g_text_segment[PC + 1];
		sprintf(instr, "int %hhx\n", type);
		pretty_print(PC + 1, 1, instr);

		PC += 2;
		syscall_hat(g_memory, g_registers);
	}
	else if (op == OP_INT_1)
	{
		pretty_print(PC + 1, 0, "int 3\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_ret(uint8_t op)
{
	if (op == OP_RET_0)
	{
		pretty_print(PC + 1, 0, "+ret\n");
		PC += 1;
		PC = pop_stack(BIT_16);
		//²// i don't know... so I hardcoded that
		//²if (PC == 0)
		//²	PC += 1;

	}
	else if (op == OP_RET_1)
	{
		char instr[32];
		uint16_t data = g_text_segment[PC + 1] + (g_text_segment[PC + 2] << 8);
		sprintf(instr, "ret %04hx\n", data);
		pretty_print(PC + 1, 2, instr);

		PC += 3;
	}
	else
		return 0;
	return 1;
}

int op_cbw(uint8_t op)
{
	if (op == OP_CBW)
	{
		pretty_print(PC + 1, 0, "+cbw\n");
		// sign extend
		g_registers[AX] = (g_registers[AX] & 0x0080 ? 0xFF00 : 0x0000) | (g_registers[AX] & 0X00FF);
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_cwd(uint8_t op)
{
	if (op == OP_CWD)
	{
		pretty_print(PC + 1, 0, "cwd\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_sub_2(uint8_t byte1)
{
	if (IS_OP(byte1, W_MASK, OP_W_SUB_2))
	{
		uint8_t w;
		uint16_t data;
		w = W(byte1);
		char instr[32];

		data = g_text_segment[PC + 1];
		if (w == 1)
		{
			data |= g_text_segment[PC + 2] << 8;
			PC += 3;
		}
		else
			PC += 2;
		sprintf(instr, "sub %s, %04hx\n", w ? "ax" : "al", data);
		pretty_print(PC + 1, 1 + w, instr);
	}
	else
		return 0;
	return 1;
}

int op_ssb_2(uint8_t byte1)
{
	if (IS_OP(byte1, OP_SSB_2_MASK, OP_SSB_2))
	{
		uint8_t w;
		uint16_t data;
		w = W(byte1);

		char instr[32];
		data = g_text_segment[PC + 1];
		if (w == 1)
		{
			data |= g_text_segment[PC + 2] << 8;
			PC += 3;
		}
		else
			PC += 2;
		sprintf(instr, "sbb %s, %04hx\n", w ? "ax" : "al", data);
		pretty_print(PC + 1, 1 + w, instr);

	}
	else
		return 0;
	return 1;
}

int op_logic(uint8_t byte1)
{
	if (IS_OP(byte1, VW_MASK, OP_VW_LOGIC))
	{
		uint8_t v, w, byte2;
		byte2 = g_text_segment[PC + 1];
		v = D(byte1);
		w = W(byte1);
		PC += 1;

		switch (FLAG(byte2))
		{
			case OP_SHL_FLAG:
				print_mr_vw("shl", byte2, v, w);
				break;
			case OP_SHR_FLAG:
				print_mr_vw("shr", byte2, v, w);
				break;
			case OP_SAR_FLAG:
				print_mr_vw("sar", byte2, v, w);
				break;
			case OP_ROL_FLAG:
				print_mr_vw("rol", byte2, v, w);
				break;
			case OP_ROR_FLAG:
				print_mr_vw("ror", byte2, v, w);
				break;
			case OP_RCL_FLAG:
				print_mr_vw("rcl", byte2, v, w);
				break;
			case OP_RCR_FLAG:
				print_mr_vw("rcr", byte2, v, w);
				break;
			default:
				return 0;
		}
	}
	else
		return 0;
	return 1;
}

int op_clc(uint8_t op)
{
	if (op == OP_CLC)
	{
		pretty_print(PC + 1, 0, "clc\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_cmc(uint8_t op)
{
	if (op == OP_CMC)
	{
		pretty_print(PC + 1, 0, "cmc\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_stc(uint8_t op)
{
	if (op == OP_STC)
	{
		pretty_print(PC + 1, 0, "stc\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_cld(uint8_t op)
{
	if (op == OP_CLD)
	{
		pretty_print(PC + 1, 0, "cld\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_std(uint8_t op)
{
	if (op == OP_STD)
	{
		pretty_print(PC + 1, 0, "std\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_cli(uint8_t op)
{
	if (op == OP_CLI)
	{
		pretty_print(PC + 1, 0, "cli\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_sti(uint8_t op)
{
	if (op == OP_STI)
	{
		pretty_print(PC + 1, 0, "sti\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_hlt(uint8_t op)
{
	if (op == OP_HLT)
	{
		pretty_print(PC + 1, 0, "hlt\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_wait(uint8_t op)
{
	if (op == OP_WAIT)
	{
		pretty_print(PC + 1, 0, "wait\n");
		PC += 1;
	}
	else
		return 0;
	return 1;
}

int op_test_2(uint8_t byte1)
{
	char instr[32];
	if (IS_OP(byte1, W_MASK, OP_W_TEST_2))
	{
		uint8_t w = W(byte1);
		uint16_t data;
		if (w == 1)
		{
			data = g_text_segment[PC + 1] | (g_text_segment[PC + 2] << 8);
			PC += 3;
			sprintf(instr, "test ax, %04hx\n", data);
		}
		else
		{
			data = g_text_segment[PC + 1];
			PC += 2;
			printf("test al, %i\n", (int8_t)data);
		}
		pretty_print(PC + 1, 1 + w, instr);
	}
	else
		return 0;
	return 1;
}

char *to_op_string(uint8_t byte)
{
	char *sub_inst;
	switch (byte & W_MASK)
	{
		case OP_W_MOVS:
			sub_inst = "movs";
			break;
		case OP_W_CMPS:
			sub_inst = "cmps";
			break;
		case OP_W_SCAS:
			sub_inst = "scas";
			break;
		case OP_W_LODS:
			sub_inst = "lods";
			break;
		case OP_W_STOS:
			sub_inst = "stos";
			break;
		default:
			sub_inst = 0;
			break;
	}
	return sub_inst;
}

int op_string(uint8_t byte1)
{
	char *action = to_op_string(byte1);
	if (action)
	{
		char type = W(byte1) ? 'w' : 'b';
		char instr[32];
		sprintf(instr, "%s%c\n", action, type);
		pretty_print(PC, 0, instr);
		PC += 1;
		return 1;
	}
	else
		return 0;
}

int op_rep(uint8_t byte1)
{
	if (IS_OP(byte1, W_MASK, OP_Z_REP))
	{
		uint8_t z, byte2, w;
		z = W(byte1);
		byte2 = g_text_segment[PC + 1];
		w = W(byte2);

		char instr[32];
		char *sub_inst = to_op_string(byte2);
		sprintf(instr, "rep %s%c\n", sub_inst,
			w ? 'w' : 'b');
		pretty_print(PC + 1, 1, instr);
		PC += 1;
	}
	else
		return 0;
	return 1;
}
