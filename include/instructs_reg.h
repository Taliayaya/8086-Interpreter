#include "instructs.h"


typedef int (*reg_instruction)(uint8_t, uint8_t);


int instructs_reg_only();

// reg instructions (0 1 0 1 0 reg)

void print_reg(char *op_name, uint8_t reg, uint8_t w);


int op_push_1(uint8_t op, uint8_t reg);
int op_pop_1(uint8_t op, uint8_t reg);
int op_xchg_1(uint8_t op, uint8_t reg);
int op_inc_1(uint8_t op, uint8_t reg);
int op_dec_1(uint8_t op, uint8_t reg);

