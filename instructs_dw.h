# include "instructs.h"

typedef int (*DW_Instruction)(uint8_t **, uint8_t, uint8_t, 
	uint8_t, uint8_t);

int op_dw(uint8_t **text_segment);

int op_mov_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_add_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_adc_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_sub_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_ssb_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_cmp_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_and_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_or_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);

int op_xor_0(uint8_t **text_segment, uint8_t op, 
	uint8_t byte2, uint8_t d, uint8_t w);
