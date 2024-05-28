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
		printf("%s, %s\n",
			source, dest);
	}
	else
		printf("%s, %s\n", 
			dest, source);
		
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

void print_mr_vw(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	 uint8_t v, uint8_t w)
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

	printf("	%s %s, %s\n", op_name, dest, v ? "cl" : "1");
		
	free(dest);

}

void print_mr_sw(uint8_t **text_segment, char *op_name, uint8_t byte2,
	uint8_t s, uint8_t w, uint8_t digit)
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

	uint16_t data = text_segment[0][0];
	printf("%02hhx", text_segment[0][0]);
	if (s == 0 && w == 1)
	{
		printf("%02hhx", text_segment[0][1]);
		data |= text_segment[0][1] << 8;
		(*text_segment) += 2;
		printf("	%s %s %s, %04hx\n", 
			op_name, w ? "" : "byte", dest, data);

	}
	else
	{
		(*text_segment) += 1;
		printf("	%s %s %s, ", 
			op_name, w ? "" : "byte", dest);
		if (digit && dest[1] == 'x' && (data & 0x80))
			printf("%hi\n", (int8_t)data);
		else
			printf("%hx\n", data);

	}

		
	free(dest);

}


void print_mr_data(uint8_t **text_segment, char *op_name, uint8_t byte2,
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

	uint16_t data;
	if (w == 1)
	{
		printf("%02hhx%02hhx", **text_segment, text_segment[0][1]);
		data = **text_segment | (text_segment[0][1] << 8);
		(*text_segment) += 2;
		printf("	%s %s, %04hx\n", op_name, dest, data);
	}
	else
	{
		printf("%02hhx", **text_segment);
		data = **text_segment;
		(*text_segment) += 1;

		printf("	%s byte %s, %2hhx\n", op_name, dest, data);
	}

		
	free(dest);
}
