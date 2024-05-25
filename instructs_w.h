#include "instructs.h"

typedef int (*W_Instruction)(uint8_t **, uint8_t, uint8_t, 
	uint8_t, uint8_t);

typedef int (*W_Instruction2)(uint8_t **, uint8_t, uint8_t);

int op_w(uint8_t **text_segment);

// W + FLAG INSTRUCTIONS

int op_mov_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_inc_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_dec_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_neg(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

// The following four op share the same op, but diff flag
int op_mul(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_imul(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_div(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_idiv(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_not(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_and_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_test_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_or_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_xor_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

// FLAG ONLY (no w)

int op_push_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_pop_0(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_call_1(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_jmp_2(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

int op_jmp_4(uint8_t **text_segment, uint8_t op, uint8_t flag,
	uint8_t byte2, uint8_t w);

