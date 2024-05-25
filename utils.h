#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>

#define _CHEAT_

#ifdef _CHEAT_

struct	exec {			/* a.out header */
  uint8_t	a_magic[2];	/* magic number */
  uint8_t	a_flags;	/* flags, see below */
  uint8_t	a_cpu;		/* cpu id */
  uint8_t	a_hdrlen;	/* length of header */
  uint8_t	a_unused;	/* reserved for future use */
  unsigned short a_version;	/* version stamp (not used at present) */
  int32_t		a_text;		/* size of text segement in bytes */
  int32_t		a_data;		/* size of data segment in bytes */
  int32_t		a_bss;		/* size of bss segment in bytes */
  int32_t		a_entry;	/* entry point */
  int32_t		a_total;	/* total memory allocated */
  int32_t		a_syms;		/* size of symbol table */

  /* SHORT FORM ENDS HERE */
  long		a_trsize;	/* text relocation size */
  long		a_drsize;	/* data relocation size */
  long		a_tbase;	/* text relocation base */
  long		a_dbase;	/* data relocation base */
};

#else

#include <a.out.h>

#endif

#define EA_STRING_SIZE 16

// RIGHT BYTE
#define MOD(x) ((x & 0b11000000) >>  6)
#define REG(x) ((x & 0b00111000) >>  3)
#define R_M(x) ((x & 0b00000111))

// LEFT BYTE
#define INSTR6(x) (x & 0b11111100 >> 2)
#define INSTR4(x) (x & 0b11110000 >> 4)

#define D(x)     ((x & 0b00000010) >> 1)
#define W(x) 	  (x & 0b00000001)

char *get_reg(uint8_t reg, int w);
char *get_r_m(uint8_t r_m);
char *get_segreg(uint8_t seg);

int get_mod(uint8_t **text_segment, uint8_t mod, uint8_t r_m, 
	uint8_t w, char *ea);



#endif
