#include "prints.h"

void print_mrr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	uint8_t d, uint8_t w)
{
	uint8_t mod, reg, r_m;
	mod = MOD(byte2);
	reg = REG(byte2);
	r_m = R_M(byte2);

	char *source, *dest;
	source = get_reg(reg, w);
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	int byte_read = get_mod(text_segment, mod, r_m, w, dest);

	printf("%02hhx", byte2);
	for (int i = 0; i < byte_read; ++i)
		printf("%02hhx", text_segment[0][i]);
	(*text_segment) += byte_read;

	printf("	%s ", op_name);
	if (d != 0)
	{
		printf("%s, %s\n", source, dest);
	}
	else
		printf("%s, %s\n", dest, source);
		
	free(dest);
}

void print_mr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	 uint8_t w)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	int byte_read = get_mod(text_segment, mod, r_m, w, dest);

	printf("%02hhx", byte2);
	for (int i = 0; i < byte_read; ++i)
		printf("%02hhx", text_segment[0][i]);
	(*text_segment) += byte_read;

	printf("	%s %s\n", op_name, dest);
		
	free(dest);

}
