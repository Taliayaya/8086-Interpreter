#ifndef PRINTS_H
#define PRINTS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

struct print_data
{
	struct mod_data mdata;
	uint16_t data;
	uint16_t mem_data;
};

struct mrr_data
{
	struct mod_data mdata;
	uint8_t reg_from : 3;
	uint8_t reg_to   : 3;
};

void print_registers_header(void);
void print_registers_state(void);

struct mod_data
print_mr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	 uint8_t w);

struct print_data
print_mr_sw(uint8_t **text_segment, char *op_name, uint8_t byte2,
	uint8_t s, uint8_t w, uint8_t digit);

struct mrr_data
print_mrr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	uint8_t d, uint8_t w);

struct print_data
print_mr_data(uint8_t **text_segment, char *op_name, uint8_t byte2,
	uint8_t w);
void print_mr_vw(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	 uint8_t v, uint8_t w);

#endif
