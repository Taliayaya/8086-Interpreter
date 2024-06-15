#include "prints.h"

void print_registers_header(void)
{
	printf(" AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP\n");
}

void print_registers_state(void)
{
	printf("%04hx %04hx %04hx %04hx %04hx %04hx %04hx %04hx ",
		g_registers[AX], g_registers[BX], g_registers[CX],
		g_registers[DX], g_registers[SP], g_registers[BP],
		g_registers[SI], g_registers[DI]);
	printf("%c%c%c%c ",
		g_flags.CF ? 'C' : '-',
		g_flags.SF ? 'S' : '-',
		g_flags.OF ? 'O' : '-',
		g_flags.ZF ? 'Z' : '-'
	);
}

struct mrr_data
print_mrr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	uint8_t d, uint8_t w)
{
	uint8_t mod, reg, r_m;
	mod = MOD(byte2);
	reg = REG(byte2);
	r_m = R_M(byte2);

	char *source, *dest;
	source = get_reg(reg, w);
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));

	struct mod_data data = get_mod(text_segment, mod, r_m, w, dest);

	struct mrr_data rdata;
	rdata.mdata = data;

	printf("%02hhx", byte2);
	for (int i = 0; i < data.byte_read; ++i)
		printf("%02hhx", text_segment[0][i]);
	(*text_segment) += data.byte_read;

	printf("	");
	printf("%s ", op_name);
	if (d != 0)
	{
		printf("%s, %s\n",
			source, dest);
		rdata.reg_from = r_m;
		rdata.reg_to = reg;
	}
	else
	{
		printf("%s, %s\n", 
			dest, source);
		rdata.reg_from = reg;
		rdata.reg_to = r_m;
	}
		
	free(dest);
	return rdata;
}

struct mod_data
print_mr(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	 uint8_t w)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data data = get_mod(text_segment, mod, r_m, w, dest);

	printf("%02hhx", byte2);
	for (int i = 0; i < data.byte_read; ++i)
		printf("%02hhx", text_segment[0][i]);
	(*text_segment) += data.byte_read;
	if (data.byte_read < 3)
		printf("	");

	printf("%s %s\n", op_name, dest);
		
	free(dest);
	return data;

}

void print_mr_vw(uint8_t **text_segment, char *op_name, uint8_t byte2, 
	 uint8_t v, uint8_t w)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data data = get_mod(text_segment, mod, r_m, w, dest);

	printf("%02hhx", byte2);
	for (int i = 0; i < data.byte_read; ++i)
		printf("%02hhx", text_segment[0][i]);
	(*text_segment) += data.byte_read;

	printf("	%s %s, %s\n", op_name, dest, v ? "cl" : "1");
		
	free(dest);

}

struct print_data
print_mr_sw(uint8_t **text_segment, char *op_name, uint8_t byte2,
	uint8_t s, uint8_t w, uint8_t digit)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data mdata = get_mod(text_segment, mod, r_m, w, dest);

	printf("%02hhx", byte2);
	for (int i = 0; i < mdata.byte_read; ++i)
		printf("%02hhx", text_segment[0][i]);
	(*text_segment) += mdata.byte_read;
	int print_tab = mdata.byte_read < 2;

	uint16_t data = text_segment[0][0];
	printf("%02hhx", text_segment[0][0]);

	char is_byte = mod <= 0b10 && !w; 

	struct print_data return_data;
	return_data.mdata = mdata;
	if (s == 0 && w == 1)
	{
		printf("%02hhx", text_segment[0][1]);
		if (print_tab)
			printf("	");
		data |= text_segment[0][1] << 8;
		(*text_segment) += 2;

		int16_t pos = *(int16_t *)(g_memory + mdata.ea);
			// setting flags
		printf("%s %s, %04hx ;%s%04hx\n", 
			op_name, dest, data, dest, pos);
		return_data.mem_data = pos;

	}
	else
	{
		(*text_segment) += 1;
		if (print_tab)
			printf("	");

		printf("%s%s %s, ", 
			op_name, !is_byte ? "" : " byte", dest);
		if (data & 0x80)
			printf("%hi ", (int8_t)data);
		else
			printf("%hx ", data);
		if (w == 1)
		{
			int16_t pos = *(int16_t *)(g_memory + mdata.ea);
			printf(";%s%hx\n", dest, pos);
			return_data.mem_data = pos;

		}
		else
		{
			int8_t pos = g_memory[mdata.ea];
			printf(";%s%hhx\n", dest, pos);
			return_data.mem_data = pos;
		}

	}
	free(dest);
	return_data.data = data;
	return return_data;

}


struct print_data
print_mr_data(uint8_t *text_segment, uint16_t *pc, char *op_name, uint8_t byte2,
	uint8_t w)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data mdata = get_mod(text_segment, mod, r_m, w, dest);

	printf("%02hhx", byte2);
	for (int i = 0; i < mdata.byte_read; ++i)
		printf("%02hhx", text_segment[0][i]);
	*pc += mdata.byte_read;
	int print_tab = mdata.byte_read < 2;
	

	struct print_data rdata; // return data
	rdata.mdata = mdata;
	uint16_t data;
	if (w == 1)
	{
		printf("%02hhx%02hhx", **text_segment, text_segment[0][1]);
		if (mdata.byte_read < 1)
			printf("	");

		data = **text_segment | (text_segment[0][1] << 8);
		*pc += 2;
		printf("%s %s, %04hx", op_name, dest, data);
	}
	else
	{
		printf("%02hhx", **text_segment);
		if (print_tab)
			printf("	");

		data = **text_segment;
		*pc += 1;
		char is_byte = mod <= 0b10;

		printf("%s %s %s, %2hhx", op_name, is_byte ? "byte" : "", dest, data);
	}

	rdata.data = data;
	if (mdata.type == MOD_EA)
	{
		printf("; %s", dest);
		if (w)
		{
			rdata.mem_data = *(int16_t *)(g_memory + rdata.mdata.ea);
			printf("%04hx", rdata.mem_data);
		}
		else
		{
			rdata.mem_data = g_memory[rdata.mdata.ea];
			printf("%02hhx", rdata.mem_data);
		}
	}
	printf("\n");

	free(dest);
	return rdata;
}
