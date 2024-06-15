#include "utils.h"

uint16_t g_registers[8] = {0, 0, 0, 0,
						  STACK_CAPACITY, 0, 0, 0};
int8_t *g_memory;
int8_t g_stack[STACK_CAPACITY] = {0,};
struct flags g_flags = {0,};
uint8_t *g_text_segment;
uint16_t PC;

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
get_mod(uint8_t *text_segment uint16_t *pc, 
	uint8_t mod, uint8_t r_m, uint8_t w,
	char *ea)
{
	uint16_t disp;
	if (mod == 0b00 && r_m == 0b110)
	{
		disp = text_segment[*pc] + (text_segment[*pc + 1] << 8);
		sprintf(ea, "[%04hx]", disp);
		return (struct mod_data){MOD_EA, 2, disp, 0};
	}
	switch (mod)
	{
		case 0b00:
			sprintf(ea, "[%s]",  get_r_m(r_m));
			return (struct mod_data){MOD_EA, 0, get_disp(r_m, 0), 0};
		case 0b01:
			disp = text_segment[*pc];
			uint8_t adisp = (disp & 0x80) ? (~disp + 1) : disp;
			sprintf(ea, "[%s%c%hhx]", get_r_m(r_m),
				(disp & 0x80) ? '-' : '+', adisp);
			return (struct mod_data){MOD_EA, 1, get_disp(r_m, adisp), 0};
		case 0b10:
			disp = (int16_t)((text_segment[*pc + 1] << 8) | text_segment[*pc]);

			int16_t adisp16 = (disp & 0x8000) ? (~disp + 1) : disp;

			char *ealeft = get_r_m(r_m);
			char sign = (disp & 0x8000) ? '-' : '+';
			sprintf(ea, "[%s%c%hx]", ealeft,
				sign, adisp16);
			return (struct mod_data){MOD_EA, 0, get_disp(r_m, adisp16), 2};

		case 0b11:
			sprintf(ea, "%s", get_reg(r_m, w));
			return (struct mod_data){MOD_REG, 0, 0, r_m};
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
		registers[reg] = (registers[reg] & 0xFF00) | (value & 0x00FF);
	}
}

int16_t 
get_registers(uint16_t registers[8], uint8_t reg, uint8_t w)
{
	return w ? registers[reg] : registers[reg] & 0x00FF;
}

void
set_memory(int8_t *memory, uint16_t ea, int16_t data, uint8_t w)
{
	if (w)
		*(int16_t *)(g_memory + ea) = data;
	else
		g_memory[ea] = data;

}

int16_t
get_memory(int8_t *memory, uint16_t ea, uint8_t w)
{
	if (w)
		return *(int16_t *)(g_memory + ea);
	else
		return g_memory[ea];

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
	set_registers(g_registers, reg, data, w);
}

void push_mem_stack(uint8_t ea, uint8_t w)
{
	uint16_t data = get_memory(g_memory, ea, w);
	push_stack(data, w);
}

void pop_mem_stack(uint8_t ea, uint8_t w)
{
	uint16_t data = pop_stack(w);
	set_memory(g_memory, ea, data, w);

}


