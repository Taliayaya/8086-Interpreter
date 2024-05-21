#include "utils.h"

char *get_reg(uint8_t reg, int w)
{
	char *regs16b[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
	char *regs8b[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

	if (w == 0)
		return regs8b[reg];
	else
		return regs16b[reg];
}

char *get_r_m(uint8_t r_m)
{
	char *disp[] = {"bx+si", "bx+di", "bp+si", "bp+di",
					"si", 	 "di",    "bp",    "bx"};
	return disp[r_m];
}

int get_mod(uint8_t **text_segment, uint8_t mod, uint8_t r_m, uint8_t w,
	char *ea)
{
	uint16_t disp;
	if (mod == 0b00 && r_m == 0b110)
	{
		disp = text_segment[0][0] + (text_segment[0][1] << 8);
		sprintf(ea, "[%04hx]", disp);
		return 2;
	}
	switch (mod)
	{
		case 0b00:
			sprintf(ea, "[%s]", get_r_m(r_m));
			return 0;
		case 0b01:
			disp = text_segment[0][0];
			uint8_t adisp = (disp & 0x80) ? (~disp + 1) : disp;
			sprintf(ea, "[%s%c%hhx]", get_r_m(r_m),
				(disp & 0x80) ? '-' : '+', adisp);
			return 1;
		case 0b10:
			disp = (int16_t)((text_segment[0][1] << 8) | text_segment[0][0]);

			int16_t adisp16 = (disp & 0x8000) ? (~disp + 1) : disp;

			char *ealeft = get_r_m(r_m);
			char sign = (disp & 0x8000) ? '-' : '+';
			sprintf(ea, "[%s%c%hx]", ealeft,
				sign, adisp16);
			return 2;

		case 0b11:
			sprintf(ea, "%s", get_reg(r_m, w));
			return 0;
	}
}
