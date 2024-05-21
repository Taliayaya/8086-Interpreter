#ifndef INSTRUCTS_H
#define INSTRUCTS_H


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

// MACRO FUNCTIONS

#define IS_OP(op, mask, flag) ((op & mask) == flag)
#define IS_DW_OP(op, flag) 	(IS_OP(op, DW_MASK, flag))
#define IS_SW_OP(op, flag) 	(IS_OP(op, SW_MASK, flag))
#define IS_W_OP(op, flag) 	(IS_OP(op, W_MASK, flag))

// MOV OPERATIONS
#define OP_DW_MOV_0		0b10001000
#define OP_W_MOV_1		0b11000110
#define OP_MOV_2		0b10110000
#define OP_MOV_2_MASK	0b11110000


// PUSH OPERATIONS
#define OP_PUSH_0 		0b11111111
#define OP_PUSH_0_DATA  0b00110000
#define OP_PUSH_0_DMASK 0b00111000
#define OP_PUSH_1 		0b01010000
#define OP_PUSH_1_MASK  0b11111000
#define OP_PUSH_2 		0b00000111
#define OP_PUSH_2_MASK 	0b11100111

// POP OPERATIONS
#define OP_POP_0		0b10001111
#define OP_POP_0_DATA	0b00000000
#define OP_POP_0_DMASK	0b00111000
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
#define OP_ADD_1		0b10000000
#define OP_ADD_1_MASK	0b11111100
#define OP_ADD_1_DATA	0b00000000
#define OP_ADD_1_DMASK	0b00111000
#define OP_ADD_2		0b00000100
#define OP_ADD_2_MASK	0b11111110

// ADC = Add with Carry
#define OP_DW_ADC_0		0b00010000
#define OP_SW_ADC_1		0b10000000
#define OP_SW_ADC_1_MSK 0b00010000
#define OP_W_ADC_2		0b00010100

// INC = Increment
#define OP_W_INC_0		0b11111110
#define OP_W_INC_0_MSK	0b00000000

#define OP_AAA			0b00110111
#define OP_BAA			0b00100111

// SUB = Subtract
#define OP_DW_SUB_0		0b00101000
#define OP_SW_SUB_1		0b10000000
#define OP_SW_SUB_1_MSK 0b00101000
#define OP_W_SUB_2		0b00101100

// SSB = Subtract with Borrow
#define OP_DW_SSB_0		0b00011000
#define OP_SW_SSB_1		0b10000000
#define OP_SSB_2		0b00011100
#define OP_SSB_2_MASK	0b11111100

// DEC = Decrement
#define OP_W_DEC_0		0b11111100
#define OP_W_DEC_0_MSK	0b00001000
#define OP_DEC_1		0b01001000
#define OP_DEC_1_MASK	0b11111000

#define OP_W_NEG		0b11110110
#define OP_W_NEG_MSK	0b00011000

// CMP = Compare
#define OP_DW_CMP_0		0b00111000
#define OP_SW_CMP_1		0b10000000
#define OP_SW_CMP_1_MSK	0b00111000
#define OP_W_CMP_2		0b00111100

#define OP_AAS			0b00111111
#define OP_DAS			0b00101111
#define OP_W_MUL		0b11110110
#define OP_W_MUL_MSK	0b00100000
#define OP_W_IMUL		0b11110110
#define OP_W_IMUL_MSK	0b00101000

// 2 BYTES OP
#define OP_AAM_0		0b11010100
#define OP_AAM_1		0b00001010

#define OP_W_DIV		0b11110110
#define OP_W_DIV_MSK	0b00110000
#define OP_W_IDIV		0b11110110
#define OP_W_IDIV_MSK	0b00111000
// 2 BYTES OP
#define OP_AAD_0		0b11010101
#define OP_AAD_1		0b00001010

#define OP_CBW			0b10011000
#define OP_CWD			0b10011001

// LOGIC
#define OP_W_NOT		0b11110110
#define OP_W_NOT_MSK	0b00010000

// AND = And
#define OP_DW_AND_0		0b00100000

// OR = Or
#define OP_DW_OR_0		0b00001000

// XOR = Exclusive or
#define OP_DW_XOR_0		0b00110000



// CALL OPERATIONS
#define OP_CALL_0		0b11101000
#define OP_CALL_1		0b11111111
#define OP_CALL_1_DATA  0b00010000
#define OP_CALL_1_DMASK 0b00111000
// JUMP OPERATIONS
#define OP_JMP_0		0b11101001
#define OP_JMP_1		0b11101011
#define OP_JMP_2		0b11111111
#define OP_JMP_2_DATA	0b00100000
#define OP_JMP_2_DMASK	0b00111000
#define OP_JMP_3		0b11101010

// TODO:
// Regroup SW together, W togethers, DW togethers, etc.
// and done!
// whats currently done is too overwhelming...

#endif
