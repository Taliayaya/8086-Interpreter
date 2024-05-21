#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

#include "file_utils.h"

#define _CHEAT_

#ifdef _CHEAT_

struct	exec {			/* a.out header */
  unsigned char	a_magic[2];	/* magic number */
  unsigned char	a_flags;	/* flags, see below */
  unsigned char	a_cpu;		/* cpu id */
  unsigned char	a_hdrlen;	/* length of header */
  unsigned char	a_unused;	/* reserved for future use */
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

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	
	char *path = argv[1];
	int fd = open(path, O_RDONLY);
	char *content;
	read_whole_file(fd, &content);

	struct exec *header = (struct exec *)content;

	//printf("%li\n", header->a_text);
	//printf("%hhu %hhu\n", header->a_magic[0], header->a_magic[1]);
	printf("%hhu %hhu %hhu %hhu %hhu %hhu\n",
		header->a_magic[0], header->a_magic[1], header->a_flags, header->a_cpu, header->a_hdrlen, header->a_unused);
	printf("%i\n", header->a_text);
	printf("%i %i %i %i %i %i\n", header->a_text,	
                                  header->a_data,	
                                  header->a_bss,	
                                  header->a_entry,
                                  header->a_total,
                                  header->a_syms);	
	char *text_area = content + header->a_hdrlen;
	for (int32_t i = 0; i < header->a_text; ++i)
		printf("%c(%hhu)", text_area[i], text_area[i]);
	char *data_area = text_area + header->a_text;
	printf("\n");
	for (int32_t i = 0; i < header->a_data; ++i)
		printf("%c(%hhu)", data_area[i], data_area[i]);


	return 0;	
}
