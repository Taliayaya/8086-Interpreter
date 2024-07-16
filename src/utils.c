#include "utils.h"

uint16_t g_registers[8] = {0, 0, 0, 0,
						  0xffff, 0, 0, 0};
int8_t *g_memory;
int8_t *g_stack;
struct flags g_flags = {0,};
uint8_t *g_text_segment;
uint16_t PC;

enum program_mode PROGRAM_MODE;


char *get_reg(uint8_t reg, int w)
{
	char *regs16b[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
	char *regs8b[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

	if (w == 0)
		return regs8b[reg];
	else
		return regs16b[reg];
}



uint16_t get_disp(uint8_t r_m, int16_t disp)
{
	//printf("disp: %hx %hi=", disp, disp);
	switch (r_m)
	{
		case 0b000:
			return GET_REGISTER(g_registers, BX, BIT_16) +
				   GET_REGISTER(g_registers, SI, BIT_16) + disp;
		case 0b001:
			return GET_REGISTER(g_registers, BX, BIT_16) +
				   GET_REGISTER(g_registers, DI, BIT_16) + disp;
		case 0b010:
			return GET_REGISTER(g_registers, BP, BIT_16) + 
				   GET_REGISTER(g_registers, SI, BIT_16) + disp;
		case 0b011:
			return GET_REGISTER(g_registers, BP, BIT_16) + 
				   GET_REGISTER(g_registers, DI, BIT_16) + disp;
		case 0b100:
			return GET_REGISTER(g_registers, SI, BIT_16) + disp;
		case 0b101:
			return GET_REGISTER(g_registers, DI, BIT_16) + disp;
		case 0b110:
			return GET_REGISTER(g_registers, BP, BIT_16) + disp;
		case 0b111:
			return GET_REGISTER(g_registers, BX, BIT_16) + disp;
		default:
			printf("ERROR: INVALID R_M!! Received %hhx\n", r_m);
			exit(1);

	}
}

char *get_r_m(uint8_t r_m)
{
	char *disp[] = {"bx+si", "bx+di", "bp+si", "bp+di",
					"si", 	 "di",    "bp",    "bx"};
	return disp[r_m];
}

char *get_segreg(uint8_t seg)
{
	char *seg_reg[] = {"es", "cs", "ss", "ds"};
	return seg_reg[seg];
}

struct mod_data
get_mod( 
	uint8_t mod, uint8_t r_m, uint8_t w,
	char *ea)
{
	struct operation_data mem_data;
	uint16_t disp;
	if (mod == 0b00 && r_m == 0b110)
	{
		disp = g_text_segment[PC] + (g_text_segment[PC + 1] << 8);
		sprintf(ea, "[%04hx]", disp);

		mem_data.type = MOD_EA;
		mem_data.data.ea = disp;
		return (struct mod_data){.byte_read=2, .memory=mem_data};
	}
	switch (mod)
	{
		case 0b00:
			sprintf(ea, "[%s]",  get_r_m(r_m));
			mem_data.type = MOD_EA;
			mem_data.data.ea = get_disp(r_m, 0);
			return (struct mod_data){.byte_read=0, .memory=mem_data};

		case 0b01:
			disp = g_text_segment[PC];
			uint8_t adisp = (disp & 0x80) ? (~disp + 1) : disp;
			sprintf(ea, "[%s%c%hhx]", get_r_m(r_m),
				(disp & 0x80) ? '-' : '+', adisp);

			mem_data.type = MOD_EA;
			mem_data.data.ea = get_disp(r_m, 
				(disp & 0x80) ? -adisp : adisp);
			return (struct mod_data){.byte_read=1, .memory=mem_data};

		case 0b10:
			disp = (int16_t)((g_text_segment[PC + 1] << 8) | g_text_segment[PC]);
			int16_t adisp16 = (disp & 0x8000) ? (~disp + 1) : disp;

			char *ealeft = get_r_m(r_m);
			char sign = (disp & 0x8000) ? '-' : '+';
			sprintf(ea, "[%s%c%hx]", ealeft,
				sign, adisp16);

			mem_data.type = MOD_EA;
			mem_data._ea = get_disp(r_m, disp);
			return (struct mod_data){.byte_read=2, .memory=mem_data};

		case 0b11:
			sprintf(ea, "%s", get_reg(r_m, w));
			mem_data.type = MOD_REG;
			mem_data.data.reg = r_m;
			return (struct mod_data){.byte_read=0, .memory=mem_data};
		default:
			err(EXIT_FAILURE, "utils.c::get_mod() - Invalid case");
	}
}

void
set_registers(uint16_t registers[8], uint8_t reg, uint8_t w, uint16_t value)
{
	if (w)
	{
		registers[reg] = value;
	}
	else
	{
		int higher_byte = (reg & 0b100) == 0b100;
		reg &= 0b011; // clear flag
		if (higher_byte) // higher byte
			registers[reg] = (registers[reg] & 0x00FF) | (value & 0xFF00);
		else // lower byte
			registers[reg] = (registers[reg] & 0xFF00) | (value & 0x00FF);

	}
}

int16_t 
get_registers(uint16_t registers[8], uint8_t reg, uint8_t w)
{
	if (w)
		return registers[reg];
	else
	{
		int higher_byte = (reg & 0b100) == 0b100;
		reg &= 0b011; // clear flag
		if (higher_byte) // higher byte
			return (registers[reg] & 0xFF00) >> 8;
		else // lower byte
			return (registers[reg] & 0x00FF);
	}

}

void
set_memory(int8_t *memory, uint16_t ea, uint8_t w, int16_t data)
{
	if (w)
		*(int16_t *)(memory + ea) = data;
	else
		memory[ea] = data;

}

int16_t
get_memory(int8_t *memory, uint16_t ea, uint8_t w)
{
	if (w)
		return *(int16_t *)(memory + ea);
	else
		return memory[ea];

}

uint16_t get_data(struct operation_data data, uint8_t w)
{
	switch (data.type)
	{
		case MOD_REG:
			return get_registers(g_registers, data._reg, w);
		case MOD_EA:
			return get_memory(g_memory, data._ea, w);
		case MOD_IMM_16:
			return data._imm16;
		case MOD_IMM_8:
			return data._imm8;
		default:
			err(EXIT_FAILURE, "utils.c::get_data() - Unknown data type");
	}
}

void set_data(struct operation_data data, uint8_t w, uint16_t value)
{
	switch (data.type)
	{
		case MOD_REG:
			set_registers(g_registers, data._reg, w, value);
			break;
		case MOD_EA:
			set_memory(g_memory, data._ea, w, value);
			break;
		case MOD_IMM_16:
			__attribute__((fallthrough));
		case MOD_IMM_8:
			err(EXIT_FAILURE, "utils.c::get_data() - Set unavailable for immediate");

			break;
		default:
			err(EXIT_FAILURE, "utils.c::get_data() - Unknown set data type");
			break;
	}
}

void push_stack(uint16_t data, uint8_t w)
{
	// we decrement sp then push
	uint16_t sp = g_registers[SP] - 1 - w;
	g_registers[SP] = sp;
	if (w)
		*(uint16_t *)(g_stack + sp) = data;
	else
		g_stack[sp] = data;
}

uint16_t pop_stack(uint8_t w)
{
	// we pop then increment sp
	uint16_t sp = g_registers[SP];
	uint16_t data;
	if (w)
		data = *(uint16_t *)(g_stack + sp);
	else
		data = g_stack[sp];

	g_registers[SP] = sp + 1 + w;
	return data;
}

void push_reg_stack(uint8_t reg, uint8_t w)
{
	uint16_t data = get_registers(g_registers, reg, w);
	push_stack(data, w);
}
void pop_reg_stack(uint8_t reg, uint8_t w)
{
	uint16_t data = pop_stack(w);
	set_registers(g_registers, reg,  w, data);
}

void push_mem_stack(uint16_t ea, uint8_t w)
{
	uint16_t data = get_memory(g_memory, ea, w);
	push_stack(data, w);
}

void pop_mem_stack(uint16_t ea, uint8_t w)
{
	uint16_t data = pop_stack(w);
	set_memory(g_memory, ea, w, data);
}

void update_sf(struct flags *flags, int16_t result, uint8_t w)
{
	flags->SF = w ? IS_NEG16(result) : IS_NEG8(result);	
}

void update_pf(struct flags *flags, int16_t result)
{
    int parity_count = 0;
    unsigned char ls_byte = result & 0xFF;
    for (int i = 0; i < 8; ++i)
	{
        if (ls_byte & (1 << i)) 
            parity_count++;
    }
	flags->PF = parity_count % 2 == 0;
}

void update_zf(struct flags *flags, int16_t result)
{
	flags->ZF = result == 0;
}

