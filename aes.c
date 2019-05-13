#include <stdio.h>
#include "aes_def.h"
#include <limits.h>
#include <float.h>

int main(int argc, char **argv)
{

	//printf("in main\n");
	BYTE llave[16];
	WORD entrada[44];
	/*
	for (char i = 0; i < 16; i++)
	{
		llave[i]=i;
		//printf("%i \n", llave[i]);
	}
	*/
	llave[0] = 0x2b;
	llave[1] = 0x7e;
	llave[2] = 0x15;
	llave[3] = 0x16;
	llave[4] = 0x28;
	llave[5] = 0xae;
	llave[6] = 0xd2;
	llave[7] = 0xa6;
	llave[8] = 0xab;
	llave[9] = 0xf7;
	llave[10] = 0x15;
	llave[11] = 0x88;
	llave[12] = 0x09;
	llave[13] = 0xcf;
	llave[14] = 0x4f;
	llave[15] = 0x3c;

	KeyExpansion(llave, entrada, 4);
	//printf("CHAR_BIT    :   %d\n", CHAR_BIT);
}

void KeyExpansion(BYTE key[4 * Nk], WORD w[Nb * (Nr + 1)], int Nk1)
{
	printf("in key expansion\n");
	WORD temp;
	int i = 0;
	while (i < Nk)
	{
		unsigned int x;
		/*
		key[3] = (w[i] >> 24) & 0xFF;
		key[2] = (w[i] >> 16) & 0xFF;
		key[1] = (w[i] >> 8) & 0xFF;
		key[0] = w[i] & 0xFF;
		*/
		w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8 | (key[4 * i + 3] << 0));
		printf("%x \n", w[i]);
		i = i + 1;
		//w[i] = WORD(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
	}
	i = Nk;
	while (i < Nb * (Nr + 1))
	{
		temp = w[i - 1];
		printf("temp = %x \n", temp);
		if(i % Nk == 0);
		
	}
	printf("out key expansion\n");
}