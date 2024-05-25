# include "instructs.h"

typedef int (*SW_Instruction)(uint8_t **, uint8_t, uint8_t, 
	uint8_t, uint8_t, uint8_t);

int op_sw(uint8_t **text_segment);

int op_add_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w);
int op_adc_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w);
int op_sub_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w);
int op_ssb_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w);
int op_cmp_1(uint8_t **text_segment, uint8_t op, uint8_t flag, 
	uint8_t byte2, uint8_t s, uint8_t w);
