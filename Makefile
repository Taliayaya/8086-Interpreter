CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
LDFLAGS =
LDLIBS =

SRC = main.c file_utils.c prints.c utils.c instructs_dw.c instructs_w.c instructs_reg.c instructs_sw.c instructs.c syscalls.c

OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: main 

main: ${OBJ} 
nomain: file_utils.o prints.o utils.o instructs_dw.o instructs_w.o instructs_reg.o instructs_sw.o instructs.o

.PHONY: clean
clean: 
	${RM} ${OBJ} ${EXE}
