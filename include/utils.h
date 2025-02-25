#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "color.h"

#define _CHEAT_

#ifdef _CHEAT_

struct	exec {			/* a.out header */
  uint8_t	a_magic[2];	/* magic number */
  uint8_t	a_flags;	/* flags, see below */
  uint8_t	a_cpu;		/* cpu id */
  uint8_t	a_hdrlen;	/* length of header */
  uint8_t	a_unused;	/* reserved for future use */
  unsigned short a_version;	/* version stamp (not used at present) */
  int32_t		a_text;		/* size of text segement in bytes */
  int32_t		a_data;		/* size of data segment in bytes */
  int32_t		a_bss;		/* size of bss segment in bytes */
  int32_t		a_entry;	/* entry point */
  int32_t		a_total;	/* total memory allocated */
  int32_t		a_syms;		/* size of symbol table */

  /* SHORT FORM ENDS HERE */
  long		a_trsize;	/* text relocation size */
  long		a_drsize;	/* data relocation size */
  long		a_tbase;	/* text relocation base */
  long		a_dbase;	/* data relocation base */
};

#else

#include <a.out.h>

#endif

#define EA_STRING_SIZE 16

#define OP_DONE_MARK ""

// RIGHT BYTE
#define MOD(x) ((x & 0b11000000) >>  6)
#define REG(x) ((x & 0b00111000) >>  3)
#define R_M(x) ((x & 0b00000111))

// LEFT BYTE
#define INSTR6(x) (x & 0b11111100 >> 2)
#define INSTR4(x) (x & 0b11110000 >> 4)

#define D(x)     ((x & 0b00000010) >> 1)
#define W(x) 	  (x & 0b00000001)

// w = 1
#define AX 0b000
#define CX 0b001
#define DX 0b010
#define BX 0b011
#define SP 0b100
#define BP 0b101
#define SI 0b110
#define DI 0b111

// w = 0 - lower byte
#define AL AX
#define BL BX
#define CL CX
#define DL DX
// w = 0 - higher byte
#define AH SP
#define BH BP
#define CH SI
#define DH DI

#define GET_REGISTER(g_regs, reg, w) (w ? g_regs[reg] : g_regs[reg] & 0x00FF)
#define BIT_16 1
#define BIT_8  0

#define IS_NEG16(x) (((x) & 0x8000) == 0x8000)
#define IS_NEG8(x)	(((x) & 0x80)	== 0x80)
#define IS_NEG(x, w) (w ? IS_NEG16(x) : IS_NEG8(x))

#define POSPOS_NEG(left, right, result, w) (!IS_NEG(left, w) && !IS_NEG(right, w) &&  IS_NEG(result, w)) 
#define NEGNEG_POS(left, right, result, w) ( IS_NEG(left, w) &&  IS_NEG(right, w) && !IS_NEG(result, w))
#define OVERFLOW(left, right, result, w) (POSPOS_NEG(left, right, result, w) || NEGNEG_POS(left, right, result, w))

#define NOT_IMPLEMENTED(op) 								\
({															\
	if (PROGRAM_MODE != DISSASSEMBLE)						\
	{														\
		if (PROGRAM_MODE == INTERPRET_DEBUG)				\
		{													\
			fprintf(stderr, "\n");							\
		}													\
		fprintf(stderr, RED"/!\\ "BOLDRED"[ "op" ]"			\
		RESET RED" was not implemented. "					\
			"Unexpected behavior may occur.\n"RESET);		\
	}														\
})

#define SIGN_EXTEND16(x) (IS_NEG8(x) ? 0xFF00 | (x) : (x) & 0x00FF)

#define STACK_CAPACITY 0xFFFF

struct op_w_flags
{
	uint8_t op : 7;
	uint8_t w  : 1;
};

struct op_no_flags
{
	uint8_t op;
};

struct op_dw_flags
{
	uint8_t op : 6;
	uint8_t  d : 1;
	uint8_t  w : 1;
};

struct op_sw_flags
{
	uint8_t op : 6;
	uint8_t  s : 1;
	uint8_t  w : 1;
};

struct op_byte2_flags
{
	uint8_t mod : 2;
	uint8_t reg : 3;
	uint8_t rm  : 3;
};

struct flags
{
	uint8_t OF : 1; // Overflow 		| 3
	uint8_t DF : 1; //
	uint8_t IF : 1; //
	uint8_t TF : 1; //
	uint8_t SF : 1; // Negative		 	| 2
	uint8_t ZF : 1; // Zero 			| 4
	uint8_t AF : 1;
	uint8_t PF : 1;
	uint8_t CF : 1; // Carry			| 1
};

enum mod_data_type
{
	MOD_EA,
	MOD_REG,
	MOD_IMM_16,
	MOD_IMM_8
};

enum program_mode
{
	DISSASSEMBLE,
	INTERPRET_DEBUG,
	INTERPRET
};

// shortcuts for union
#define _reg	data.reg
#define _ea		data.ea
#define _imm8	data.immediate_8
#define _imm16 	data.immediate_16

struct operation_data
{
	enum mod_data_type type;
	union
	{
		uint8_t reg : 3;
		uint16_t ea;
		uint16_t immediate_16;
		uint8_t immediate_8;
	} data;
};



struct mod_data
{
	uint8_t byte_read : 2;
	struct operation_data memory;
};


// GLOBALS (its bad)
extern uint16_t g_registers[8];
extern int8_t *g_memory;
extern int8_t *g_stack;
extern struct flags g_flags;
extern uint8_t *g_text_segment;
extern uint16_t PC;

extern enum program_mode PROGRAM_MODE;

char *get_reg(uint8_t reg, int w);
char *get_r_m(uint8_t r_m);
char *get_segreg(uint8_t seg);

struct mod_data get_mod(uint8_t mod, uint8_t r_m, 
	uint8_t w, char *ea);

uint16_t get_disp(uint8_t r_m, int16_t disp);

// UTILS

// INLINE
void
set_registers(uint16_t registers[8], uint8_t reg, uint8_t w, uint16_t value);

int16_t 
get_registers(uint16_t registers[8], uint8_t reg, uint8_t w);


void
set_memory(int8_t *memory, uint16_t ea, uint8_t w, int16_t data);


int16_t
get_memory(int8_t *memory, uint16_t ea, uint8_t w);

uint16_t 
get_data(struct operation_data data, uint8_t w);

void 
set_data(struct operation_data data, uint8_t w, uint16_t value);

void push_stack(uint16_t data, uint8_t w);
uint16_t pop_stack(uint8_t w);

void push_reg_stack(uint8_t reg, uint8_t w);
void pop_reg_stack(uint8_t reg, uint8_t w);

void push_mem_stack(uint16_t ea, uint8_t w);
void pop_mem_stack(uint16_t ea, uint8_t w);

void update_sf(struct flags *flags, int16_t result, uint8_t w);
void update_pf(struct flags *flags, int16_t result);
void update_zf(struct flags *flags, int16_t result);

#endif
