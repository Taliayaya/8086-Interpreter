#ifndef PRINTS_H
#define PRINTS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

struct print_data
{
	struct operation_data data_left;
	struct operation_data data_right;
};

void print_registers_header(void);
void print_registers_state(void);

void pretty_print(uint16_t byte_start, size_t count, char *instr);


struct operation_data
print_mr(char *op_name, uint8_t byte2, 
	 uint8_t w);

struct print_data
print_mr_sw(char *op_name, uint8_t byte2,
	uint8_t s, uint8_t w);

struct print_data
print_mrr(char *op_name, uint8_t byte2, 
	uint8_t d, uint8_t w);

struct print_data
print_mr_data(char *op_name, uint8_t byte2,
	uint8_t w);

struct operation_data
print_mr_vw(char *op_name, uint8_t byte2, 
	 uint8_t v, uint8_t w);


struct calc_data
{
	uint16_t left, right, result;
};

enum calc_type
{
	CALC_XOR,
	CALC_OR,
	CALC_AND,
	CALC_ADD,
	CALC_SUB,
	CALC_XCHG
};

uint16_t compute_calc(enum calc_type type, uint16_t a, uint16_t b);
struct calc_data process_operation(struct print_data pdata, enum calc_type type, 
	int d, int w);


#endif
