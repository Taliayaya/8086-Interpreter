#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct a
{
	union
	{
		uint8_t b;
		uint16_t c;
	} u;
};

int main()
{
	struct a m;
	m.u.b = 10;
	printf("%hhi", m.u.c);
}
