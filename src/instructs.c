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

	return index < length;

}

// PUSH
int op_push_2(uint8_t byte1)
{
	if (IS_OP(byte1, OP_PUSH_2_MASK, OP_PUSH_2))
	{
		uint8_t reg = byte1 & 0b00011000;
		char instr[32];
		sprintf(instr, "push %s", get_segreg(reg));
		pretty_print(PC + 1, 1, instr);
		PC += 1;


		NOT_IMPLEMENTED("push_2");
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
		sprintf(instr, "pop %s", get_segreg(reg));
		pretty_print(PC + 1, 1, instr);

		NOT_IMPLEMENTED("pop_2");
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
		sprintf(instr, "in %s, %02hhx", w ? "ax" : "al", port);
		pretty_print(PC + 1, 1, instr);
		PC += 2;
	}
	else if (IS_OP(byte1, OP_IN_1_MASK, OP_IN_1))
	{
		sprintf(instr, "in %s, dx", w ? "ax" : "al");
		pretty_print(PC + 1, 0, instr);
		PC += 1;
	}
	else
		return 0;
	NOT_IMPLEMENTED("in");
	return 1;
}

int op_out(uint8_t byte1)
{
	uint8_t w = W(byte1);
	char instr[32];
	if (IS_OP(byte1, OP_OUT_0_MASK, OP_OUT_0))
	{
		uint8_t port = g_text_segment[PC + 1];
		sprintf(instr, "out %s, %02hhx", w ? "ax" : "al", port);
		pretty_print(PC + 1, 1, instr);

		PC += 2;
	}
	else if (IS_OP(byte1, OP_OUT_1_MASK, OP_OUT_1))
	{
		sprintf(instr, "out %s, dx", w ? "ax" : "al");
		pretty_print(PC + 1, 0, instr);
		PC += 1;
	}
	else
		return 0;
	NOT_IMPLEMENTED("out");
	return 1;
}


// LOAD & FLAGS STUFF
int op_xlat(uint8_t op)
{
	if (op == OP_XLAT)
	{
		pretty_print(PC + 1, 0, "xlat");

		PC += 1;
		NOT_IMPLEMENTED("xlat");
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
		rdata = print_mrr(OP_DONE_MARK"lea", byte2, 1, 1);
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
		
		NOT_IMPLEMENTED("lds");
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

		NOT_IMPLEMENTED("les");
		return 1;
	}
	else
		return 0;
}

int op_lahf(uint8_t op)
{
	if (op == OP_LAHF)
	{
		pretty_print(PC + 1, 0, OP_DONE_MARK"lahf");
		PC += 1;
		// NOT SUPPORTED IN 16 BITS
		//uint8_t ah = (g_flags.SF << 7) | (g_flags.ZF << 6) | (g_flags.CF);
		//set_registers(g_registers, AH, BIT_8, ah);

		return 1;
	}
	else
		return 0;
}

int op_sahf(uint8_t op)
{
	if (op == OP_SAHF)
	{
		pretty_print(PC + 1, 0, "sahf");
		PC += 1;

		NOT_IMPLEMENTED("sahf");
		return 1;
	}
	else
		return 0;
}

int op_pushf(uint8_t op)
{
	if (op == OP_PUSHF)
	{
		pretty_print(PC + 1, 0, "pushf");
		PC += 1;

		NOT_IMPLEMENTED("pushf");
		return 1;
	}
	else
		return 0;
}

int op_popf(uint8_t op)
{
	if (op == OP_POPF)
	{
		pretty_print(PC + 1, 0, "popf");
		PC += 1;

		NOT_IMPLEMENTED("popf");
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
			data |= (g_text_segment[PC + 2] << 8);

		set_registers(g_registers, reg, w, data);

		char instr[32];
		sprintf(instr, OP_DONE_MARK"mov %s, %04hx", get_reg(reg, w), data);
		pretty_print(PC + 1, 2, instr);
		PC += 2 + w;
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


		char instr[32];
		sprintf(instr, OP_DONE_MARK"call %04hx", PC + 3 + increment);
		pretty_print(PC + 1, 2, instr);

		if (PROGRAM_MODE == DISSASSEMBLE)
		{
			PC += 3;
			return;
		}
		push_stack(PC + 3, BIT_16); // Push next instruction
		PC += increment + 3; // go to function

		return 1;	
	}
	else
		return 0;
}

// JUMPS
int op_jmp(uint8_t op)
{
	uint16_t pc;
	if (op == OP_JMP_0)
	{
		uint8_t byte1, byte2;
		byte1 = g_text_segment[PC + 1];
		byte2 = g_text_segment[PC + 2];
		int16_t data16b = (byte2 << 8) | byte1;		
		uint16_t target = PC + 3 + data16b;

		char instr[32];
		sprintf(instr, OP_DONE_MARK"jmp %04hx", target);
		pretty_print(PC + 1, 2, instr);
		PC += 3;
		pc = target;
	}
	else if (op == OP_JMP_1)
	{
		int8_t disp = g_text_segment[PC + 1];
		char instr[32];
		sprintf(instr, OP_DONE_MARK"jmp short %04hx", PC + 2 + disp);
		pretty_print(PC + 1, 1, instr);

		pc = PC + 2 + disp;
		PC += 2;
	}
	else
		return 0;
	if (PROGRAM_MODE != DISSASSEMBLE)
		PC = pc;
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
			op_name = OP_DONE_MARK"je";
			can_jump = g_flags.ZF;
			break;
		case OP_JL:
			op_name = OP_DONE_MARK"jl";
			can_jump = g_flags.SF != g_flags.OF;
			break;
		case OP_JLE:
			op_name = OP_DONE_MARK"jle";
			can_jump = g_flags.ZF || (g_flags.SF != g_flags.OF);
			break;
		case OP_JB:
			op_name = OP_DONE_MARK"jb";
			can_jump = g_flags.CF;
			break;
		case OP_JBE:
			op_name = OP_DONE_MARK"jbe";
			can_jump = g_flags.CF || g_flags.ZF;
			break;
		case OP_JP:
			op_name = "jl";
			NOT_IMPLEMENTED("jl");
			break;
		case OP_JO:
			op_name = "jo";
			NOT_IMPLEMENTED("jo");
			break;
		case OP_JS:
			op_name = "js";
			NOT_IMPLEMENTED("js");
			break;
		case OP_JNE:
			op_name = OP_DONE_MARK"jne";
			can_jump = !g_flags.ZF;
			break;
		case OP_JNL:
			op_name = OP_DONE_MARK"jnl";
			can_jump = g_flags.SF == g_flags.OF;
			break;
		case OP_JNLE:
			op_name = OP_DONE_MARK"jnle";
			can_jump = !g_flags.ZF && (g_flags.OF == g_flags.SF);
			break;
		case OP_JNB:
			op_name = OP_DONE_MARK"jnb";
			can_jump = !g_flags.CF;
			break;
		case OP_JNBE:
			op_name = OP_DONE_MARK"jnbe";
			can_jump = !g_flags.CF && !g_flags.ZF;
			break;
		case OP_JNP:
			op_name = "jnp";
			NOT_IMPLEMENTED("jnp");
			break;
		case OP_JNO:
			op_name = "jno";
			NOT_IMPLEMENTED("jno");
			break;
		case OP_JNS:
			op_name = "jns";
			NOT_IMPLEMENTED("jns");
			break;
		case OP_LOOP:
			op_name = "loop";
			NOT_IMPLEMENTED("loop");
			break;
		case OP_LOOPZ:
			op_name = "loopz";
			NOT_IMPLEMENTED("loopz");
			break;
		case OP_LOOPNZ:
			op_name = "loopnz";
			NOT_IMPLEMENTED("loopnz");
			break;
		case OP_JCXZ:
			op_name = OP_DONE_MARK"jcxz";
			can_jump = g_registers[CX] == 0;
			break;

		default:
			return 0;
	}
	
	char instr[32];
	sprintf(instr, "%s %04hx", op_name, PC + 2 + disp);
	pretty_print(PC + 1, 1, instr);

	PC += 2;
	if (can_jump && PROGRAM_MODE != DISSASSEMBLE)
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
		sprintf(instr, OP_DONE_MARK"int %hhx", type);
		pretty_print(PC + 1, 1, instr);

		PC += 2;
		if (PROGRAM_MODE != DISSASSEMBLE)
		{
			syscall_hat(g_memory, g_registers);
		}
	}
	else if (op == OP_INT_1)
	{
		pretty_print(PC + 1, 0, "int 3");
		PC += 1;
		NOT_IMPLEMENTED("int 3");
	}
	else
		return 0;
	return 1;
}

int op_ret(uint8_t op)
{
	if (op == OP_RET_0)
	{
		pretty_print(PC + 1, 0, OP_DONE_MARK"ret");
		PC += 1;
		if (PROGRAM_MODE != DISSASSEMBLE)
			PC = pop_stack(BIT_16);

	}
	else if (op == OP_RET_1)
	{
		char instr[32];
		uint16_t data = g_text_segment[PC + 1] + (g_text_segment[PC + 2] << 8);
		sprintf(instr, OP_DONE_MARK"ret %04hx", data);
		pretty_print(PC + 1, 2, instr);

		PC += 3;
		if (PROGRAM_MODE != DISSASSEMBLE)
			PC = pop_stack(BIT_16);
		g_registers[SP] += data;// * 2;
	}
	else
		return 0;
	return 1;
}

int op_cbw(uint8_t op)
{
	if (op == OP_CBW)
	{
		pretty_print(PC + 1, 0, OP_DONE_MARK"cbw");
		// sign extend
		g_registers[AX] = SIGN_EXTEND16(g_registers[AX]);
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
		pretty_print(PC + 1, 0, OP_DONE_MARK"cwd");
		PC += 1;
		uint16_t ax_reg = get_registers(g_registers, AX, BIT_16);
		// sign extend AX to DX
		if (IS_NEG16(ax_reg))
			set_registers(g_registers, DX, BIT_16, 0xFFFF);
		else
			set_registers(g_registers, DX, BIT_16, 0x0000);
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
		uint16_t rdata;
		w = W(byte1);
		char instr[32];

		rdata = g_text_segment[PC + 1];
		if (w == 1)
		{
			rdata |= g_text_segment[PC + 2] << 8;
			PC += 3;
		}
		else
			PC += 2;
		sprintf(instr, OP_DONE_MARK"sub %s, %04hx", w ? "ax" : "al", rdata);
		pretty_print(PC + 1, 1 + w, instr);

		uint16_t ldata = get_registers(g_registers, AX, w);
		uint16_t result = ldata - rdata;
		set_registers(g_registers, AX, w, result);

		g_flags.OF = OVERFLOW(ldata, rdata, result, w);
		g_flags.SF = IS_NEG(result, w); // negative
		g_flags.ZF = result == 0; // zero
		g_flags.CF = ldata < rdata;

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
		sprintf(instr, "sbb %s, %04hx", w ? "ax" : "al", data);
		pretty_print(PC + 1, 1 + w, instr);

		NOT_IMPLEMENTED("sbb");

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

		uint8_t count = v ? get_registers(g_registers, CL, BIT_8) : 1;

		struct operation_data data;

		uint16_t val, res;

		switch (FLAG(byte2))
		{
			case OP_SHL_FLAG:
				data = print_mr_vw(OP_DONE_MARK"shl", byte2, v, w);
				val = get_data(data, w);
				res = val << count;
				g_flags.CF = (res & ((w ? 0x8000 : 0x80) >> (count - 1))) == 1;
				//if (count == 1) // error coming from the ref?
				// this should be done only for 1 bit shifts
					g_flags.OF = (w ? IS_NEG16(res) : IS_NEG8(res)) != g_flags.CF;
				break;
			case OP_SHR_FLAG:
				data = print_mr_vw("shr", byte2, v, w);
				NOT_IMPLEMENTED("shr");
				break;
			case OP_SAR_FLAG:
				data = print_mr_vw(OP_DONE_MARK"sar", byte2, v, w);
				val = get_data(data, w);
				res = w ? val : SIGN_EXTEND16(val);
				uint16_t sign_extend = IS_NEG16(res) ? 0x8000 : 0;
				for (int i = 0; i < count; ++i)
				{
					g_flags.CF = res & 0x0001;
					res  >>= 1;
					res |= sign_extend;
				}
				if (count == 1)
					g_flags.OF = 0; // cleared

				break;
			case OP_ROL_FLAG:
				data = print_mr_vw("rol", byte2, v, w);
				NOT_IMPLEMENTED("rol");
				break;
			case OP_ROR_FLAG:
				data = print_mr_vw("ror", byte2, v, w);
				NOT_IMPLEMENTED("ror");
				break;
			case OP_RCL_FLAG:
				data = print_mr_vw("rcl", byte2, v, w);
				NOT_IMPLEMENTED("rcl");
				break;
			case OP_RCR_FLAG:
				data = print_mr_vw("rcr", byte2, v, w);
				NOT_IMPLEMENTED("rcr");
				break;
			default:
				return 0;
		}

		set_data(data, w, res);
		g_flags.ZF = res == 0;
		g_flags.SF = IS_NEG(res, w);
	}
	else
		return 0;
	return 1;
}

int op_clc(uint8_t op)
{
	if (op == OP_CLC)
	{
		pretty_print(PC + 1, 0, "clc");
		PC += 1;

		NOT_IMPLEMENTED("clc");
	}
	else
		return 0;
	return 1;
}

int op_cmc(uint8_t op)
{
	if (op == OP_CMC)
	{
		pretty_print(PC + 1, 0, "cmc");
		PC += 1;

		NOT_IMPLEMENTED("cmc");
	}
	else
		return 0;
	return 1;
}

int op_stc(uint8_t op)
{
	if (op == OP_STC)
	{
		pretty_print(PC + 1, 0, "stc");
		PC += 1;
		NOT_IMPLEMENTED("stc");
	}
	else
		return 0;
	return 1;
}

int op_cld(uint8_t op)
{
	if (op == OP_CLD)
	{
		pretty_print(PC + 1, 0, OP_DONE_MARK"cld");
		PC += 1;

		// clear DF flag
		g_flags.DF = 0;
	}
	else
		return 0;
	return 1;
}

int op_std(uint8_t op)
{
	if (op == OP_STD)
	{
		pretty_print(PC + 1, 0, "std");
		PC += 1;

		NOT_IMPLEMENTED("std");
	}
	else
		return 0;
	return 1;
}

int op_cli(uint8_t op)
{
	if (op == OP_CLI)
	{
		pretty_print(PC + 1, 0, "cli");
		PC += 1;

		NOT_IMPLEMENTED("cli");
	}
	else
		return 0;
	return 1;
}

int op_sti(uint8_t op)
{
	if (op == OP_STI)
	{
		pretty_print(PC + 1, 0, "sti");
		PC += 1;

		NOT_IMPLEMENTED("sti");
	}
	else
		return 0;
	return 1;
}

int op_hlt(uint8_t op)
{
	if (op == OP_HLT)
	{
		pretty_print(PC + 1, 0, "hlt");
		PC += 1;

		NOT_IMPLEMENTED("hlt");
	}
	else
		return 0;
	return 1;
}

int op_wait(uint8_t op)
{
	if (op == OP_WAIT)
	{
		pretty_print(PC + 1, 0, "wait");
		PC += 1;

		NOT_IMPLEMENTED("wait");
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
			sprintf(instr, "test ax, %04hx", data);
		}
		else
		{
			data = g_text_segment[PC + 1];
			PC += 2;
			printf("test al, %i", (int8_t)data);
		}
		pretty_print(PC + 1, 1 + w, instr);

		NOT_IMPLEMENTED("test_2");
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
		sprintf(instr, "%s%c", action, type);
		pretty_print(PC, 0, instr);

		NOT_IMPLEMENTED("string instances");
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
		sprintf(instr, "rep %s%c", sub_inst,
			w ? 'w' : 'b');
		pretty_print(PC + 1, 1, instr);
		PC += 1;

		NOT_IMPLEMENTED("rep");
	}
	else
		return 0;
	return 1;
}
