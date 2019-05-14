#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "aes_def.h"
#include "aes_var.h"

int main(int argc, char **argv){

	//printf("in main\n");
	BYTE llave[16];
	WORD subLlaves[44];
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

	KeyExpansion(llave, subLlaves, 4);
	for (int j = 0; j < sizeof(subLlaves)/sizeof(subLlaves[0]); j++)
	{
		printf("w[%i] = %x \n",j, subLlaves[j]);
	}
	
	//printf("CHAR_BIT    :   %d\n", CHAR_BIT);
}

void KeyExpansion(BYTE key[4 * Nk], WORD w[Nb * (Nr + 1)], int Nk1){
	printf("in key expansion\n");
	WORD temp;
	int i = 0;
	while (i < Nk){
		unsigned int x;
		/*
		key[3] = (w[i] >> 24) & 0xFF;
		key[2] = (w[i] >> 16) & 0xFF;
		key[1] = (w[i] >> 8) & 0xFF;
		key[0] = w[i] & 0xFF;
		*/
		w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8 | (key[4 * i + 3] << 0));
		//printf("%x \n", w[i]);
		i = i + 1;
		//w[i] = WORD(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
	}
	//printf("w[0] = %x \n", w[0]);
	//printf("w[1] = %x \n", w[1]);
	//printf("w[2] = %x \n", w[2]);
	//printf("w[3] = %x \n", w[3]);
	i = Nk;
	while (i < Nb * (Nr + 1)){
		temp = w[i - 1];
		//printf("temp = %x \n", temp);
		if(i % Nk == 0){
			RotWord(&temp);
			//printf("temp con rotword = %x \n", temp);
			SubWord(&temp);
			//printf("con subword = %x \n", temp);
			temp = temp ^ Rcon[i/Nk] ;
			//printf("con xor Rcon = %x \n", temp);
			//printf("i / Nk = %x \n", i/Nk);
		}else if ((Nk>6) && (i%Nk ==4)){  
			SubWord(&temp);
		}
		w[i] = w[i-Nk] ^ temp;
		//printf("w[%i] = %x \n",i, w[i]);
		i = i + 1;
	}
	printf("out key expansion\n");
}

void RotWord(WORD *temp){
	*temp = ((*temp<<8) & 0xFFFFFF00) | ((*temp >>24 ) & 0x000000FF);
}

void SubBytes(BYTE *indice){
	int aux = 0;
	BYTE indic[2];
	indic[0] = (*indice & 0x0F) ;
	//printf("subByte indic [0] = %x \n",indic[0]);
	indic[1] = ((*indice >> 4) & 0x0F) ;
	//printf("subByte indic [1] = %x \n",indic[1]);
	aux = S_Box[indic[0]][indic[1]];
	*indice = aux;
	//*temp = ((*temp<<8) & 0xFFFFFF00) | ((*temp >>24 ) & 0x000000FF);
}

void SubWord(WORD *fourBytes){
	BYTE aux[4];
	aux[0]= (*fourBytes >> 0 )& 0x000000FF;
	//printf("subword aux 0 = %x \n",aux[0]);
	SubBytes(&aux[0]);

	aux[1]= (*fourBytes >> 8 ) & 0x000000FF;
	//printf("subword aux 1 = %x \n",aux[1]);
	SubBytes(&aux[1]);

	aux[2]= (*fourBytes >> 16 ) & 0x000000FF;
	//printf("subword aux 2 = %x \n",aux[2]);
	SubBytes(&aux[2]);

	aux[3]= (*fourBytes >> 24 ) & 0x000000FF;
	//printf("subword aux 3 = %x \n",aux[3]);
	SubBytes(&aux[3]);

	*fourBytes = ((aux[0] << 0) | (aux[1] << 8) |(aux[2] << 16) |(aux[3] << 24));
}