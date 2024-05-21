CC = gcc
CFLAGS = -Wall -Wextra -g 
LDFLAGS =
LDLIBS =

SRC = main.c file_utils.c prints.c utils.c instructs.c

OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

all: main 

main: ${OBJ} 

.PHONY: clean
clean: 
	${RM} ${OBJ} ${EXE}
