#ifndef _LED_H_
#define _LED_H_

#include <absacc.h>

#define CSDS 0xf030 // adres bufora wyboru wskaznika/ow
#define CSDB 0xf038 // adres bufora danej

void napisz(char cyfra, char wyswietlacz, bit kropka)
{
	static char cyfry[10] = {
		0x3F, 0x06, 0x5B,
		0x4F, 0x66, 0x6D,
		0x7D, 0x07, 0x7F,
		0x6F
	};

	/*if(cyfra > 9 || wyswietlacz > 7)
	{
		P1_7 = 0;
		return;
	}*/

	P1_6 = 1;
	XBYTE[CSDS] = 1 << (wyswietlacz - 1); // wybor wskaznika
	XBYTE[CSDB] = (kropka ? cyfry[cyfra] | 0x80 : cyfry[cyfra]);
	P1_6 = 0;
}

#endif