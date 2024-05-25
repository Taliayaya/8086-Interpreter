#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <stdint.h>

#include "file_utils.h"
#include "utils.h"
#include "instructs.h"

// instructions
#include "instructs_dw.h"
#include "instructs_sw.h"
#include "instructs_w.h"
#include "instructs_reg.h"
#include "instructs_dw.h"

//int instruct_move(uint8_t **text_segment, uint8_t op)
//{
//	if ((op & OP_MOV_1_MASK) == OP_MOV_1)
//	{
//		uint8_t d, w, byte2;
//		d = D(op);
//		w = W(op);
//		byte2 = text_segment[0][1];
//		(*text_segment) += 2;
//		print_mrr(text_segment, "	mov", byte2, d, w);
//	}
//	else if ((op & OP_MOV_2_MASK) == OP_MOV_2)
//		printf("		(undefined)\n");
//	else if ((op & OP_MOV_3_MASK) == OP_MOV_3)
//	{
//		uint8_t w = (op & 0b00001000) >> 3;
//		uint8_t reg = R_M(op);
//		uint8_t data = text_segment[0][1];
//		if (w == 1)
//		{
//			uint8_t data2 = text_segment[0][2];
//			printf("%02hhx%02hhx		mov %s, %02hhx%02hhx\n",
//				data, data2, get_reg(reg, 1), data2, data);
//			(*text_segment) += 3;
//		}
//		else
//		{
//			printf("%02hhx		mov %s, %02hhx\n",
//				data, get_reg(reg, 0), data);
//			(*text_segment) += 2;
//		}
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_push(uint8_t **text_segment, uint8_t op)
//{
//	if (op == OP_PUSH_0 &&
//		(text_segment[0][1] & OP_PUSH_0_DMASK) == OP_PUSH_0_DATA)
//	{
//		uint8_t byte2 = text_segment[0][1];
//		(*text_segment) += 2;
//		print_mr(text_segment, "	push", byte2, 0);
//	} 
//	else if ((op & OP_PUSH_1_MASK) == OP_PUSH_1)
//	{
//		uint8_t reg = R_M(op);
//		printf("		push %s\n", get_reg(reg, 1));
//		(*text_segment) += 1;
//	}
//	else if ((op & OP_PUSH_2_MASK) == OP_PUSH_2)
//	{
//		printf("		(undefined)\n");
//		(*text_segment) += 1;
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_call(uint8_t **text_segment, uint8_t op, uint16_t index)
//{
//	if (op == OP_CALL_0)
//	{
//		int16_t increment = (text_segment[0][2] << 8) | text_segment[0][1];
//		printf("%02hhx%02hhx	 	call %04hx\n",  
//			text_segment[0][1], text_segment[0][2], 
//			index + increment + 3);
//		(*text_segment) += 3;
//	}
//	else if (op == OP_CALL_1 &&
//		(text_segment[0][1] & OP_CALL_1_DMASK) == OP_CALL_1_DATA)
//	{
//		printf("		(undefined)\n");
//		(*text_segment) += 2;
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_add(uint8_t **text_segment, uint8_t op)
//{
//	if ((op & OP_ADD_0_MASK) == OP_ADD_0)
//	{
//		uint8_t byte2, d, w;
//		byte2 = text_segment[0][1];
//		d = D(op);
//		w = W(op);
//		(*text_segment) += 2;
//		print_mrr(text_segment, "	add", byte2, d, w);
//
//	}
//	else if ((op & OP_ADD_1_MASK) == OP_ADD_1
//		&& REG(text_segment[0][1]) == 0b000)
//	{
//		uint8_t byte2, data, s, w;
//		byte2 = text_segment[0][1];
//		s = D(op);
//		w = W(op);
//		(*text_segment) += 2;
//		print_mr_sw(text_segment, "	add", byte2, s, w);
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_jmp(uint8_t **text_segment, uint8_t op, uint16_t index)
//{
//	if (op == OP_JMP_0)
//	{
//		uint8_t byte1, byte2;
//		byte1 = text_segment[0][1];
//		byte2 = text_segment[0][2];
//		int16_t data16b = (byte2 << 8) | byte1;		
//		int16_t target = index + 3 + data16b;
//		printf("%02hhx%02hhx		jmp %04hx\n",
//			byte1, byte2, target);
//		(*text_segment) += 3;
//	}
//	else if (op == OP_JMP_1)
//	{
//		uint8_t disp = text_segment[0][1];
//		printf("%02hhx		jmp short %04hx\n",
//			disp, index + 2 + disp);
//		(*text_segment) += 2;
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_cond_jmp(uint8_t **text_segment, uint8_t op, uint16_t index)
//{
//	int8_t disp = text_segment[0][1];
//	printf("%02hhx", disp);
//	char *op_name;
//
//	switch(op)
//	{
//		case OP_JE:
//			op_name = "je";
//			break;
//		case OP_JNE:
//			op_name = "jne";
//			break;
//		case OP_JNL:
//			op_name = "jnl";
//			break;
//		default:
//			return 0;
//	}
//
//	printf("		%s %04hx\n", op_name, index + 2 + disp);
//	(*text_segment) += 2;
//
//	return 1;
//}
//
//int op_in(uint8_t **text_segment, uint8_t op)
//{
//	if ((op & OP_IN_0_MASK) == OP_IN_0)
//	{
//		uint8_t port = text_segment[0][1];
//		char *accumulator;
//		if (W(op) == 0)
//			accumulator = "al";
//		else
//			accumulator = "ax";
//
//		printf("%02hhx		in %s, %02hhx\n",
//			port, accumulator, port);
//		(*text_segment) += 2;
//	}
//	else if ((op & OP_IN_1_MASK) == OP_IN_1)
//	{
//		char *accumulator;
//		if (W(op) == 0)
//			accumulator = "al";
//		else
//			accumulator = "ax";
//		printf("		in %s, dx\n", accumulator);
//		(*text_segment) += 1;
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_sbb(uint8_t **text_segment, uint8_t op)
//{
//	if ((op & OP_SSB_0_MASK) == OP_SSB_0)
//	{
//		uint8_t byte2, d, w;
//		byte2 = text_segment[0][1];
//		d = D(op);
//		w = W(op);
//
//		(*text_segment) += 2;
//		print_mrr(text_segment, "	sbb", byte2, d, w);
//	}
//	else if ((op & OP_SSB_0_MASK) == OP_SSB_0)
//	{
//	}
//	else
//		return 0;
//	return 1;
//
//}
//
//
//int op_load(uint8_t **text_segment, uint8_t op)
//{
//	if (op == OP_LEA)
//	{
//		uint8_t byte2;
//		byte2 = text_segment[0][1];
//		(*text_segment) += 2;
//		print_mrr(text_segment, "	lea", byte2, 1, 1);
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_pop(uint8_t **text_segment, uint8_t op)
//{
//	if (op == OP_POP_0 &&
//		(text_segment[0][1] & OP_POP_0_DMASK) == OP_POP_0_DATA)
//	{
//		uint8_t byte2 = text_segment[0][1];
//		(*text_segment) += 2;
//		print_mr(text_segment, "	pop", byte2, 0);
//
//	}
//	else if ((op & OP_POP_1_MASK) == OP_POP_1)
//	{
//		uint8_t reg = R_M(op);
//		char *sreg = get_reg(reg, 1);
//		printf("		pop %s\n", sreg);
//		(*text_segment) += 1;
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_or(uint8_t **text_segment, uint8_t op)
//{
//	if ((op & OP_OR_0_MASK) == OP_OR_0)
//	{
//		uint8_t d, w, byte2;
//		d = D(op);
//		w = W(op);
//		byte2 = text_segment[0][1];
//		(*text_segment) += 2;
//		print_mrr(text_segment, "	or", byte2, d, w);
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_cmp(uint8_t **text_segment, uint8_t op)
//{
//	if ((op & OP_CMP_0_MASK) == OP_CMP_0)
//	{
//		uint8_t byte2, d, w;
//		byte2 = text_segment[0][1];
//		d = D(byte2);
//		w = W(byte2);
//		(*text_segment) += 2;
//		print_mrr(text_segment, "cmp", byte2, d, w);
//	}
//	else if ((op & OP_CMP_1_MASK) == OP_CMP_1 
//		&& REG(text_segment[0][1]) == 0b111)
//	{
//		uint8_t byte2, s, w;
//		byte2 = text_segment[0][1];
//		s = D(byte2);
//		w = W(byte2);
//		(*text_segment) += 2;
//		print_mr_sw(text_segment, "cmp", byte2, s, w);
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_byte(uint8_t **text_segment, uint8_t op)
//{
//	if ((op & OP_NEG_MASK) == OP_NEG)
//	{
//		uint8_t byte2, w;
//		byte2 = text_segment[0][1];
//		w = W(op),
//		(*text_segment) += 2;
//		print_mr(text_segment, "	neg", byte2, w);
//	}
//	else
//		return 0;
//	return 1;
//}
//
//int op_int(uint8_t **text_segment, uint8_t op)
//{
//	if (op == OP_INT_0)
//	{
//		uint8_t type = text_segment[0][1];
//		printf("		int %hhx\n", type);
//		(*text_segment) += 2;
//	}
//	else if (op == OP_INT_1)
//	{
//		printf("		int 3\n");
//		(*text_segment) += 1;
//	}
//	else
//		return 0;
//	return 1;
//
//}
//
//int op_ret(uint8_t **text_segment, uint8_t op)
//{
//	if (op == OP_RET_0)
//	{
//		(*text_segment) += 1;
//		printf("		ret\n");
//	}
//	else
//		return 0;
//	return 1;
//}


void instruct(uint8_t **text_segment, long index)
{
	uint8_t op = text_segment[0][0];

	printf("%04lx: %02hhx", index, op);
	if (op_dw(text_segment))
		return;
	else if (op_sw(text_segment))
		return;
	else if (op_w(text_segment))
		return;
	else if (instructs_reg_only(text_segment))
		return;
	else if (op_others(text_segment, index))
		return;
	else if (op_pc(text_segment, index))
		return;
//	else if (op_w2(text_segment))
//		return;
	(*text_segment) += 2;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	
	char *path = argv[1];
	int fd = open(path, O_RDONLY);
	char *content;
	read_whole_file(fd, &content);

	struct exec *header = (struct exec *)content;

	uint8_t *text_area = content + header->a_hdrlen;
	uint8_t *current_text = text_area;
	while (current_text < text_area + header->a_text - 1)
	{
		//printf("%02hhX ", text_area[i]);
		instruct(&current_text, current_text - text_area);
	}
	printf("%04lx: 00		(undefined)\n", current_text - text_area);
	uint8_t *data_area = text_area + header->a_text;
	printf("\n");
	for (int32_t i = 0; i < header->a_data; ++i)
		printf("%c", data_area[i]);


	return 0;	
}
