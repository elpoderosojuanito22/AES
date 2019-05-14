#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "aes_def.h"
#include "aes_var.h"

int main(int argc, char **argv)
{

	//printf("in main\n");
	BYTE llave[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	BYTE in[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	WORD subLlaves[44];
	BYTE out[4 * Nb];
	KeyExpansion(llave, subLlaves, 4);
	//for (int j = 0; j < sizeof(subLlaves) / sizeof(subLlaves[0]); j++)
	//{
	//	printf("w[%i] = %x \n", j, subLlaves[j]);
	//}
	Cipher(in, out, subLlaves);
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
		//printf("%x \n", w[i]);
		i = i + 1;
		//w[i] = WORD(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
	}
	//printf("w[0] = %x \n", w[0]);
	//printf("w[1] = %x \n", w[1]);
	//printf("w[2] = %x \n", w[2]);
	//printf("w[3] = %x \n", w[3]);
	i = Nk;
	while (i < Nb * (Nr + 1))
	{
		temp = w[i - 1];
		//printf("temp = %x \n", temp);
		if (i % Nk == 0)
		{
			RotWord(&temp);
			//printf("temp con rotword = %x \n", temp);
			SubWord(&temp);
			//printf("con subword = %x \n", temp);
			temp = temp ^ Rcon[i / Nk];
			//printf("con xor Rcon = %x \n", temp);
			//printf("i / Nk = %x \n", i/Nk);
		}
		else if ((Nk > 6) && (i % Nk == 4))
		{
			SubWord(&temp);
		}
		w[i] = w[i - Nk] ^ temp;
		//printf("w[%i] = %x \n",i, w[i]);
		i = i + 1;
	}
	printf("out key expansion\n");
}

void RotWord(WORD *temp)
{
	*temp = ((*temp << 8) & 0xFFFFFF00) | ((*temp >> 24) & 0x000000FF);
}

void SubBytes(BYTE *indice)
{
	int aux = 0;
	BYTE indic[2];
	indic[0] = (*indice & 0x0F);
	//printf("subByte indic [0] = %x \n",indic[0]);
	indic[1] = ((*indice >> 4) & 0x0F);
	//printf("subByte indic [1] = %x \n",indic[1]);
	aux = S_Box[indic[0]][indic[1]];
	*indice = aux;
	//*temp = ((*temp<<8) & 0xFFFFFF00) | ((*temp >>24 ) & 0x000000FF);
}

void SubWord(WORD *fourBytes)
{
	BYTE aux[4];
	aux[0] = (*fourBytes >> 0) & 0x000000FF;
	//printf("subword aux 0 = %x \n",aux[0]);
	SubBytes(&aux[0]);

	aux[1] = (*fourBytes >> 8) & 0x000000FF;
	//printf("subword aux 1 = %x \n",aux[1]);
	SubBytes(&aux[1]);

	aux[2] = (*fourBytes >> 16) & 0x000000FF;
	//printf("subword aux 2 = %x \n",aux[2]);
	SubBytes(&aux[2]);

	aux[3] = (*fourBytes >> 24) & 0x000000FF;
	//printf("subword aux 3 = %x \n",aux[3]);
	SubBytes(&aux[3]);

	*fourBytes = ((aux[0] << 0) | (aux[1] << 8) | (aux[2] << 16) | (aux[3] << 24));
}

void Cipher(BYTE in[4 * Nb], BYTE out[4 * Nb], WORD w[Nb * (Nr + 1)])
{
	BYTE state[4][Nb];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Nb; j++)
		{
			state[j][i] = in[(i * 4) + j];
			printf("%x ", state[j][i]);
		}
		printf("\n");
	}
	printf("\n");
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < Nb; j++)
	//	{
	//		printf("%x ", state[i][j]);
	//	}
	//	printf("\n");
	//}

	for (int i = 0; i < 4; i++)
	{
		printf("%x \n", w[i]);
	}
	AddRoundKey(state, w, 0, Nb - 1);
}

void AddRoundKey(BYTE state[4][Nb], WORD w[Nb * (Nr + 1)], int inicio, int termino)
{
	BYTE aux[4][4];
	for (int i = inicio; i < termino + 1; i++)
	{
		aux[0][i] = (w[i] >> 24) & 0x000000FF;
		state[0][i] = state[0][i] ^ aux[0][i];
		printf("%x ", state[0][i]);
		aux[1][i] = (w[i] >> 16) & 0x000000FF;
		state[1][i] = state[1][i] ^ aux[1][i];
		printf("%x ", state[1][i]);
		aux[2][i] = (w[i] >> 8) & 0x000000FF;
		state[2][i] = state[2][i] ^ aux[2][i];
		printf("%x ", state[2][i]);
		aux[3][i] = (w[i] >> 0) & 0x000000FF;
		state[3][i] = state[3][i] ^ aux[3][i];
		printf("%x ", state[3][i]);
		printf("\n");
	}
}