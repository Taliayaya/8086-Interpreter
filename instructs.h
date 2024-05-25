#ifndef INSTRUCTS_H
#define INSTRUCTS_H


#include <stdlib.h>
#include <stdint.h>

#include "utils.h"
#include "prints.h"

// TODO for some, data is not printed nor taken into account

// INSTRUCTIONS
#define MOV1 0b100010 
#define MOV2 0b1100011
#define MOV3 0b1011
#define MOV4 0b1010000
#define MOV5 0b1010001
#define MOV6 0b10001110
#define MOV7 0b10001100

// OP CATEGORIES
#define DW_MASK			0b11111100
#define SW_MASK			0b11111100
#define W_MASK			0b11111110
#define FLAG_MASK		0b00111000

// MACRO FUNCTIONS

#define FLAG(byte2) ((byte2 & FLAG_MASK) >> 3)
#define IS_OP(op, mask, flag) ((op & mask) == flag)
#define IS_DW_OP(op, flag) 	(IS_OP(op, DW_MASK, flag))
#define IS_SW_OP(op, flag) 	(IS_OP(op, SW_MASK, flag))
#define IS_W_OP(op, flag) 	(IS_OP(op, W_MASK, flag))

// MOV OPERATIONS
#define OP_DW_MOV_0		0b10001000
#define OP_W_MOV_1		0b11000110
#define OP_W_MOV_1_FLAG	0b000
#define OP_MOV_2		0b10110000
#define OP_MOV_2_MASK	0b11110000


// PUSH OPERATIONS
#define OP_PUSH_0 		0b11111111
#define OP_PUSH_0_FLAG	0b110
#define OP_PUSH_1 		0b01010000
#define OP_PUSH_1_MASK  0b11111000
#define OP_PUSH_2 		0b00000111
#define OP_PUSH_2_MASK 	0b11100111

// POP OPERATIONS
#define OP_POP_0		0b10001111
#define OP_POP_0_FLAG	0b000
#define OP_POP_1		0b01011000
#define OP_POP_1_MASK	0b11111000
#define OP_POP_2		0b00000111
#define OP_POP_2_MASK	0b11100111

// XCHG = Exchange
#define OP_XCHG_0		0b10000110
#define OP_XCHG_0_MASK	0b11111110
#define OP_XCHG_1		0b10010000
#define OP_XCHG_1_MASK	0b11111000

// IN = Input from
#define OP_IN_0			0b11100100
#define OP_IN_0_MASK	0b11111110
#define OP_IN_1			0b11101100
#define OP_IN_1_MASK	0b11111110

// OUT = Output to
#define OP_OUT_0		0b11100110
#define OP_OUT_0_MASK	0b11111110
#define OP_OUT_1		0b11101110
#define OP_OUT_1_MASK	0b11111110

// LOAD & FLAGS Operations
#define OP_XLAT			0b11010111
#define OP_LEA			0b10001101
#define OP_LDS			0b11000101
#define OP_LES			0b11000100
#define OP_LAHF			0b10011111
#define OP_SAHF			0b10011110
#define OP_PUSHF		0b10011100
#define OP_POPF			0b10011101

// ADD OPERATIONS
#define OP_DW_ADD_0		0b00000000
#define OP_SW_ADD_1		0b10000000
#define OP_SW_ADD_1_FLAG	0b000
#define OP_ADD_2		0b00000100
#define OP_ADD_2_MASK	0b11111110

// ADC = Add with Carry
#define OP_DW_ADC_0		0b00010000
#define OP_SW_ADC_1		0b10000000
#define OP_SW_ADC_1_FLAG 0b000
#define OP_W_ADC_2		0b00010100

// INC = Increment
#define OP_W_INC_0		0b11111110
#define OP_W_INC_0_FLAG	0b000
#define OP_INC_1		0b01000000

#define OP_AAA			0b00110111
#define OP_BAA			0b00100111

// SUB = Subtract
#define OP_DW_SUB_0		0b00101000
#define OP_SW_SUB_1		0b10000000
#define OP_SW_SUB_1_FLAG 0b00101000
#define OP_W_SUB_2		0b00101100

// SSB = Subtract with Borrow
#define OP_DW_SSB_0		0b00011000
#define OP_SW_SSB_1		0b10000000
#define OP_SW_SSB_1_FLAG	0b011
#define OP_SSB_2		0b00011100
#define OP_SSB_2_MASK	0b11111100

// DEC = Decrement
#define OP_W_DEC_0		0b11111100
#define OP_W_DEC_0_FLAG	0b111
#define OP_DEC_1		0b01001000
#define OP_DEC_1_MASK	0b11111000

#define OP_W_NEG		0b11110110
#define OP_W_NEG_FLAG	0b011

// CMP = Compare
#define OP_DW_CMP_0		0b00111000
#define OP_SW_CMP_1		0b10000000
#define OP_SW_CMP_1_FLAG	0b111
#define OP_W_CMP_2		0b00111100

#define OP_AAS			0b00111111
#define OP_DAS			0b00101111
#define OP_W_MUL		0b11110110
#define OP_W_MUL_FLAG	0b100
#define OP_W_IMUL		0b11110110
#define OP_W_IMUL_FLAG	0b101

// 2 BYTES OP
#define OP_AAM_0		0b11010100
#define OP_AAM_1		0b00001010

#define OP_W_DIV		0b11110110
#define OP_W_DIV_FLAG	0b110
#define OP_W_IDIV		0b11110110
#define OP_W_IDIV_FLAG	0b111
// 2 BYTES OP
#define OP_AAD_0		0b11010101
#define OP_AAD_1		0b00001010

#define OP_CBW			0b10011000
#define OP_CWD			0b10011001

// LOGIC
#define OP_W_NOT		0b11110110
#define OP_W_NOT_FLAG	0b010

// AND = And
#define OP_DW_AND_0		0b00100000
#define OP_W_AND_1		0b10000000
#define OP_W_AND_1_FLAG	0b100

// TEST = And Function to Flags, No Result
#define OP_W_TEST_1		0b11110110
#define OP_W_TEST_1_FLAG 0b000

// OR = Or
#define OP_DW_OR_0		0b00001000
#define OP_W_OR_1		0b10000000
#define OP_W_OR_1_FLAG	0b001

// XOR = Exclusive or
#define OP_DW_XOR_0		0b00110000
#define OP_W_XOR_1		0b10000000
#define OP_W_XOR_1_FLAG	0b110



// CALL OPERATIONS
#define OP_CALL_0		0b11101000
#define OP_W_CALL_1		0b11111111
#define OP_W_CALL_1_FLAG  0b010
#define OP_CALL_1_DMASK 0b00111000

// JUMP OPERATIONS
#define OP_JMP_0		0b11101001
#define OP_JMP_1		0b11101011
#define OP_JMP_2		0b11111111
#define OP_JMP_2_FLAG	0b100
#define OP_JMP_3		0b11101010
#define OP_JMP_4		0b11111111
#define OP_JMP_4_FLAG	0b101

// RET = Return from CALL
#define OP_RET_0		0b11000011
#define OP_RET_1		0b11000010
#define OP_RET_2		0b11001011
#define OP_RET_3		0b11001010

// CONDITIONS JUMP
#define OP_JE			0b01110100
#define OP_JL			0b11111100
#define OP_JLE			0b01111110
#define OP_JB			0b01110010
#define OP_JBE			0b01110110
#define OP_JP			0b01111010
#define OP_JO			0b01110000
#define OP_JS			0b01111000
#define OP_JNE			0b01110101
#define OP_JNL			0b01111101
#define OP_JNLE			0b01111111
#define OP_JNB			0b01110011
#define OP_JNBE			0b01110111
#define OP_JNP			0b01111011
#define OP_JNO			0b01110001
#define OP_JNS			0b01111001

// INT = Interrupt
#define OP_INT_0		0b11001101
#define OP_INT_1		0b11001100

// TODO:
// Regroup SW together, W togethers, DW togethers, etc.
// and done!
// whats currently done is too overwhelming...
int op_others(uint8_t **text_segment, uint16_t pc);
int op_pc(uint8_t **text_segment, uint16_t pc);
// MOV
int op_mov_2(uint8_t **text_segment, uint8_t byte1);

// PUSH
int op_push_2(uint8_t **text_segment, uint8_t byte1);

// POP
int op_pop_2(uint8_t **text_segment, uint8_t byte1);

// IN = Input from
int op_in(uint8_t **text_segment, uint8_t byte1);

// OUT = Output from
int op_out(uint8_t **text_segment, uint8_t byte1);

// LOAD & FLAGS STUFF
int op_xlat(uint8_t **text_segment, uint8_t op);

int op_lea(uint8_t **text_segment, uint8_t op);
int op_lds(uint8_t **text_segment, uint8_t op);
int op_les(uint8_t **text_segment, uint8_t op);

int op_lahf(uint8_t **text_segment, uint8_t op);
int op_sahf(uint8_t **text_segment, uint8_t op);
int op_pushf(uint8_t **text_segment, uint8_t op);
int op_popf(uint8_t **text_segment, uint8_t op);

// PC
int op_call_0(uint8_t **text_segment, uint8_t op, uint16_t pc);

int op_jmp(uint8_t **text_segment, uint8_t op, uint16_t pc);
int op_cond_jmp(uint8_t **text_segment, uint8_t op, uint16_t pc);

int op_int(uint8_t **text_segment, uint8_t op);
int op_ret(uint8_t **text_segment, uint8_t op);


#endif
