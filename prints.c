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
		g_flags.OF ? 'O' : '-',
		g_flags.SF ? 'S' : '-',
		g_flags.ZF ? 'Z' : '-',
		g_flags.CF ? 'C' : '-'
	);
}

void print_memory_content(struct operation_data data, uint8_t w)
{
	printf(" ;[%04hx]", data._ea);
	if (w)
	{
		int16_t mem_data = get_memory(g_memory, 
			data._ea, BIT_16);
		printf("%04hx", mem_data);
	}
	else
	{
		int16_t mem_data = get_memory(g_memory, 
			data._ea, BIT_8);
		printf("%02hhx", mem_data);
	}
}

void pretty_print(uint16_t byte_start, size_t count, char *instr)
{

 	char concatenated[2 * count + 1]; 
    concatenated[0] = '\0';

	char temp[4];

    for (size_t i = 0; i < count; i++)
    {
        sprintf(temp, "%02x", g_text_segment[byte_start + i]);

        strcat(concatenated, temp);
    }
    printf("%-14s%s", concatenated, instr);
}

struct print_data
print_mrr(char *op_name, uint8_t byte2, 
	uint8_t d, uint8_t w)
{
	uint8_t mod, reg, r_m;
	mod = MOD(byte2);
	reg = REG(byte2);
	r_m = R_M(byte2);

	char *source, *dest;
	source = get_reg(reg, w);
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));

	struct mod_data data = get_mod(mod, r_m, w, dest);
	
	struct print_data rdata;
	PC += data.byte_read;

	char instr[32];
	if (d != 0)
	{
		rdata.data_left = (struct operation_data)
			{.type=MOD_REG, ._reg=reg};
		rdata.data_right = data.memory;
	}
	else
	{

		char *tmp;
		tmp = dest;
		dest = source;
		source = tmp;

		rdata.data_right = (struct operation_data)
			{.type=MOD_REG, ._reg=reg};
		rdata.data_left = data.memory;

	}


	sprintf(instr, "%s %s, %s", op_name, source, dest);
	pretty_print(PC - 1, data.byte_read + 1, instr);

	if (data.memory.type == MOD_EA)
		print_memory_content(data.memory, w);
	printf("\n");

	return rdata;
}

struct operation_data
print_mr(char *op_name, uint8_t byte2, 
	 uint8_t w)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data data = get_mod(mod, r_m, w, dest);

	char instr[32];
	sprintf(instr, "%s %s", op_name, dest);
	pretty_print(PC - 1, data.byte_read + 1, instr);

	if (data.memory.type == MOD_EA)
		print_memory_content(data.memory, w);
	printf("\n");
		
	PC += data.byte_read;
	free(dest);
	return data.memory;

}

void print_mr_vw(char *op_name, uint8_t byte2, 
	 uint8_t v, uint8_t w)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data data = get_mod(mod, r_m, w, dest);

	char instr[32];
	sprintf(instr, "%s %s, %s\n", op_name, dest, v ? "cl" : "1");

	pretty_print(PC - 1, data.byte_read + 1, instr);
	PC += data.byte_read;
		
	free(dest);

}

struct print_data
print_mr_sw(char *op_name, uint8_t byte2,
	uint8_t s, uint8_t w, uint8_t digit)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data mdata = get_mod(mod, r_m, w, dest);


	size_t byte_read = 1 + mdata.byte_read;

	char is_byte = mod <= 0b10 && !w; 

	struct print_data return_data;
	return_data.data_left = mdata.memory;

	char instr[32];

	PC += mdata.byte_read;

	uint16_t data = g_text_segment[PC];
	if (s == 0 && w == 1)
	{
		byte_read += 2;
		data |= g_text_segment[PC + 1] << 8;
		PC += 2;

			// setting flags
		sprintf(instr, "%s %s, %04hx", 
			op_name, dest, data);
		return_data.data_right = (struct operation_data)
			{.type=MOD_IMM_16, ._imm16=data};
	}
	else
	{
		PC += 1;
		byte_read += 1;

		char data_str[4];
		sprintf(data_str, data & 0x80 ? "%hi"  : "%hx", (int8_t)data);

		return_data.data_right = (struct operation_data)
			{.type=MOD_IMM_8, ._imm8=data};

		sprintf(instr, "%s%s %s, %s", 
			op_name, !is_byte ? "" : " byte", dest, data_str);


	}

	pretty_print(PC - byte_read, byte_read, instr);
	if (mdata.memory.type == MOD_EA)
		print_memory_content(mdata.memory, w);
	printf("\n");
	free(dest);
	return return_data;

}


struct print_data
print_mr_data(char *op_name, uint8_t byte2,
	uint8_t w)
{
	uint8_t mod, r_m;
	mod = MOD(byte2);
	r_m = R_M(byte2);

	char *dest;
	dest = malloc(EA_STRING_SIZE * sizeof(unsigned char));
	struct mod_data mdata = get_mod(mod, r_m, w, dest);

	size_t byte_read = mdata.byte_read + 1;

	PC += mdata.byte_read;

	struct print_data rdata; // return data
	rdata.data_left = mdata.memory;
	uint16_t data;

	char instr[32];
	if (w == 1)
	{
		byte_read += 2;

		data = g_text_segment[PC] | (g_text_segment[PC + 1] << 8);
		rdata.data_right = (struct operation_data)
			{.type=MOD_IMM_16, ._imm16=data};
		PC += 2;
		sprintf(instr, "%s %s, %04hx", op_name, dest, data);
	}
	else
	{
		data = g_text_segment[PC];
		PC += 1;
		byte_read += 1;
		char is_byte = mod <= 0b10;
		rdata.data_right = (struct operation_data)
			{.type=MOD_IMM_8, ._imm8=data};

		sprintf(instr, "%s %s %s, %2hhx", op_name, is_byte ? "byte" : "", dest, data);
	}

	pretty_print(PC - byte_read, byte_read, instr);
	if (mdata.memory.type == MOD_EA)
		print_memory_content(rdata.data_left, w);

	printf("\n");

	free(dest);
	return rdata;
}

uint16_t compute_calc(enum calc_type type, uint16_t a, uint16_t b)
{
	switch (type)
	{
		case CALC_XOR:
			return a ^ b;
		case CALC_OR:
			return a | b;
		case CALC_AND:
			return a & b;
		case CALC_ADD:
			return a + b;
		case CALC_SUB:
			return a - b;
		default:
			printf("Unknown CALC_TYPE\n");
			exit(1);
	}
}


struct calc_data process_operation(struct print_data pdata, enum calc_type type, 
	int d, int w)
{
	uint16_t ldata, rdata, result;
	// MOD_REG == MOD_REG
	if (pdata.data_left.type == pdata.data_right.type) 	
	{
		ldata = get_registers(g_registers, pdata.data_left._reg, w);
		rdata = get_registers(g_registers, pdata.data_right._reg, w);
		result = compute_calc(type, ldata, rdata);
		set_registers(g_registers, pdata.data_left._reg, w, result);
	}
	else // MOD_EA
	{
		if (d == 0) // mem, reg
		{
			rdata = get_registers(g_registers, 
				pdata.data_right._reg, w);
			ldata = get_memory(g_memory, 
				pdata.data_left._ea, w);
			result = compute_calc(type, ldata, rdata);
			set_memory(g_memory, pdata.data_left._ea, w, result);
		}
		else // reg, mem
		{
			ldata = get_registers(g_registers, 
				pdata.data_left._reg, w);
			rdata = get_memory(g_memory, 
				pdata.data_right._ea, w);
			result = compute_calc(type, ldata, rdata);
			set_registers(g_registers, pdata.data_left._reg, w,
				result);
		}
	}
	struct calc_data res_data;
	res_data.left = ldata;
	res_data.right = rdata;
	res_data.result = result;
	return res_data;
}
