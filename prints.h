#ifndef PRINTS_H
#define PRINTS_H

#include "utils.h"

void print_mr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	 uint8_t w);
void print_mr_sw(uint8_t **text_segment, char *op_name, uint8_t byte2,
	uint8_t s, uint8_t w);

void print_mrr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	uint8_t d, uint8_t w);

#endif
