#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include "aes_def.h"
#include "aes_var.h"

int main(argc, argv) int argc;
char *argv[];
{

	//printf("in main\n");
	//BYTE llave[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	//BYTE in[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	BYTE in[16] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};
	WORD subLlaves[44];
	BYTE out[4 * Nb];
	procesa_argv(argc, argv); /* Procesa los argumentos */
	KeyExpansion(llave, subLlaves, 4);
	//Cipher(in, out, subLlaves);
	//InvCipher(in, out, subLlaves);
	CifrarDescifrarArchivo(in, subLlaves);
}

void KeyExpansion(BYTE key[4 * Nk], WORD w[Nb * (Nr + 1)], int Nk1)
{
	WORD temp;
	int i = 0;
	while (i < Nk)
	{
		unsigned int x;
		w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8 | (key[4 * i + 3] << 0));
		i = i + 1;
	}
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
	indic[1] = ((*indice >> 4) & 0x0F);
	aux = S_Box[indic[0]][indic[1]];
	*indice = aux;
}

void SubWord(WORD *fourBytes)
{
	BYTE aux[4];
	aux[0] = (*fourBytes >> 0) & 0x000000FF;
	SubBytes(&aux[0]);

	aux[1] = (*fourBytes >> 8) & 0x000000FF;
	SubBytes(&aux[1]);

	aux[2] = (*fourBytes >> 16) & 0x000000FF;
	SubBytes(&aux[2]);

	aux[3] = (*fourBytes >> 24) & 0x000000FF;
	SubBytes(&aux[3]);

	*fourBytes = ((aux[0] << 0) | (aux[1] << 8) | (aux[2] << 16) | (aux[3] << 24));
}

void Cipher(BYTE in[4 * Nb], BYTE out[4 * Nb], WORD w[Nb * (Nr + 1)])
{
	BYTE state[4][Nb];
	printf("\n---------entrada--------\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Nb; j++)
		{
			state[j][i] = in[(i * 4) + j];
			printf("%x ", state[j][i]);
		}
		printf("\n");
	}
	AddRoundKey(state, w, 0, Nb - 1);
	printf("\n----SubBytes----\n");
	for (int round = 1; round <= Nr - 1; round++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				SubBytes(&state[j][i]);
				printf("%x ", state[j][i]);
			}
			printf("\n");
		}
		ShiftRows(state);
		printf("\n----ShiftRows----\n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				printf("%x ", state[j][i]);
			}
			printf("\n");
		}
		printf("\n----mixCol----\n");
		MixColumns(state);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				printf("%x ", state[j][i]);
			}
			printf("\n");
		}
		AddRoundKey(state, w, round * Nb, ((round + 1) * Nb) - 1);
		printf("\n----SubBytes----\n \n");
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SubBytes(&state[j][i]);
			printf("%x ", state[j][i]);
		}
		printf("\n");
	}
	ShiftRows(state);
	AddRoundKey(state, w, Nr * Nb, ((Nr + 1) * Nb) - 1);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Nb; j++)
		{
			out[(i * 4) + j] = state[j][i];
			//printf("%x ", state[j][i]);
		}
		printf("\n");
	}
}

void AddRoundKey(BYTE state[4][Nb], WORD w[Nb * (Nr + 1)], int inicio, int termino)
{
	BYTE aux[4][4];
	printf("\n----llaves del round----\n");
	for (int i = inicio; i < termino + 1; i++)
	{
		printf("%x \n", w[i]);
	}
	//printf("\n --------------\n");
	printf("\n------addRoundkey------\n");
	for (int i = 0; i < 3 + 1; i++)
	{
		aux[0][i] = (w[inicio] >> 24) & 0x000000FF;
		state[0][i] = state[0][i] ^ aux[0][i];
		printf("%x ", state[0][i]);
		aux[1][i] = (w[inicio] >> 16) & 0x000000FF;
		state[1][i] = state[1][i] ^ aux[1][i];
		printf("%x ", state[1][i]);
		aux[2][i] = (w[inicio] >> 8) & 0x000000FF;
		state[2][i] = state[2][i] ^ aux[2][i];
		printf("%x ", state[2][i]);
		aux[3][i] = (w[inicio] >> 0) & 0x000000FF;
		state[3][i] = state[3][i] ^ aux[3][i];
		printf("%x ", state[3][i]);
		printf("\n");
		inicio++;
	}
}

void ShiftRows(BYTE state[4][Nb])
{
	BYTE temp;
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;
	for (int i = 0; i < 2; i++)
	{
		temp = state[2][0];
		state[2][0] = state[2][1];
		state[2][1] = state[2][2];
		state[2][2] = state[2][3];
		state[2][3] = temp;
	}
	for (int i = 0; i < 3; i++)
	{
		temp = state[3][0];
		state[3][0] = state[3][1];
		state[3][1] = state[3][2];
		state[3][2] = state[3][3];
		state[3][3] = temp;
	}
}

void MixColumns(BYTE state[4][Nb])
{
	int i;
	unsigned char Tmp, Tm, t;
	for (i = 0; i < 4; i++)
	{
		t = state[0][i];
		Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
		Tm = state[0][i] ^ state[1][i];
		Tm = xtime(Tm);
		state[0][i] ^= Tm ^ Tmp;
		Tm = state[1][i] ^ state[2][i];
		Tm = xtime(Tm);
		state[1][i] ^= Tm ^ Tmp;
		Tm = state[2][i] ^ state[3][i];
		Tm = xtime(Tm);
		state[2][i] ^= Tm ^ Tmp;
		Tm = state[3][i] ^ t;
		Tm = xtime(Tm);
		state[3][i] ^= Tm ^ Tmp;
	}
}

/*aqui comienza las funciones para el descifrado */

void InvCipher(BYTE in[4 * Nb], BYTE out[4 * Nb], WORD w[Nb * (Nr + 1)])
{
	BYTE state[4][Nb];
	printf("\n----entrada descrifrado----\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Nb; j++)
		{
			state[j][i] = in[(i * 4) + j];
			printf("%x ", state[j][i]);
		}
		printf("\n");
	}
	AddRoundKey(state, w, Nr * Nb, ((Nr + 1) * Nb) - 1);
	for (int round = Nr - 1; round >= 1; round--)
	{
		InvShiftRows(state);
		printf("\n----InvShiftRows----\n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				printf("%x ", state[j][i]);
			}
			printf("\n");
		}
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				InvSubBytes(&state[j][i]);
				printf("%x ", state[j][i]);
			}
			printf("\n");
		}
		AddRoundKey(state, w, round * Nb, ((round + 1) * Nb) - 1);
		printf("\n----InvmixCol----\n");
		InvMixColumns(state);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				printf("%x ", state[j][i]);
			}
			printf("\n");
		}
		printf("\n----SubBytes----\n \n");
	}
	InvShiftRows(state);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			InvSubBytes(&state[j][i]);
			printf("%x ", state[j][i]);
		}
		printf("\n");
	}
	AddRoundKey(state, w, 0, Nb - 1);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < Nb; j++)
		{
			out[(i * 4) + j] = state[j][i];
			//printf("%x ", state[j][i]);
		}
		printf("\n");
	}
}
void InvSubBytes(BYTE *indice)
{
	int aux = 0;
	BYTE indic[2];
	indic[0] = (*indice & 0x0F);
	indic[1] = ((*indice >> 4) & 0x0F);
	aux = Inv_S_Box[indic[0]][indic[1]];
	*indice = aux;
}

void InvShiftRows(BYTE state[4][Nb])
{
	BYTE temp;
	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;
	for (int i = 0; i < 2; i++)
	{
		temp = state[2][3];
		state[2][3] = state[2][2];
		state[2][2] = state[2][1];
		state[2][1] = state[2][0];
		state[2][0] = temp;
	}
	for (int i = 0; i < 3; i++)
	{
		temp = state[3][3];
		state[3][3] = state[3][2];
		state[3][2] = state[3][1];
		state[3][1] = state[3][0];
		state[3][0] = temp;
	}
}

void InvMixColumns(BYTE state[4][Nb])
{
	int i;
	unsigned char a, b, c, d;
	for (i = 0; i < 4; i++)
	{
		a = state[0][i];
		b = state[1][i];
		c = state[2][i];
		d = state[3][i];
		state[0][i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
		state[1][i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
		state[2][i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
		state[3][i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
	}
}

void CifrarDescifrarArchivo(BYTE in[16], WORD w[Nb * (Nr + 1)])
{
	int i;
	BYTE out[4 * Nb];
	while (1)
	{ /* Hasta terminar de leer */
		if ((i = fread((char *)in, 1, 16, fpe)) != 16)
		{ /* Lee bloque de 64 bits */
			if (i == 0)
				break; /* Termino de leer, todo bien */
			else if ((i > 0) && (i < 16))
				for (; i < 16; i++)
					((char *)in)[i] = 0x0; /* Relleno */
			else
			{ /* Hubo un error en la lectura */
				fprintf(stderr, "AES-Error: Error al leer del archivo\n");
				exit(1);
			}
		}
		if (cifrar)
		{
			Cipher(in, out, w);
		}
		else
		{
			InvCipher(in, out, w);
		}
		if (fwrite((char *)out, 1, 16, fps) != 16)
		{ /* Escribe el bloque cifrado */
			fprintf(stderr, "AES-Error: Error al escribir al archivo\n");
			exit(1);
		}
	}
}

void procesa_argv(argc, argv) int argc;
char *argv[];
{
	if (argc != 5)
		uso();
	strcpy(ent, argv[4]);
	strcpy(sal, argv[4]);
	char hex[2];
	for (int i = 0; i < 16; i++) //lee la llave desde argumentos de la terminal
	{
		hex[0] = argv[2][i*2];
		hex[1] = argv[2][i*2 +1];
		BYTE num = (int)strtol(hex, NULL, 16);
		llave[i]=num;
		printf("%x ", num);
	}

	if (argv[1][0] == '-')
	{
		switch (argv[1][1])
		{
		case 'c':
			strcat(sal, ".cif");
			break;
		case 'd':
			//case 'e':
			cifrar = FALSE;
			if (strcmp(".cif", &ent[strlen(argv[4]) - 4]))
				strcat(ent, ".cif");
			else
				sal[strlen(argv[4]) - 4] = 0;
			if (argv[1][1] == 'd')
				//borrar = FALSE;
				break;
		//case 'p':
		//	//cifrar = FALSE;
		//	if (strcmp(".cif", &ent[strlen(argv[4]) - 4]))
		//		strcat(ent, ".cif");
		//	//pantalla = TRUE;
		//	fps = stdout; /* La salida a la pantalla */
		//	break;
		default:
			fprintf(stderr, "AES-Error: opcion ilegal %c\n", argv[1][1]);
			exit(1);
		}
	}
	else
		uso();
	if ((fpe = fopen(ent, "r")) == NULL)
	{
		fprintf(stderr, "AES-Error: no puedo abrir %s\n", ent);
		exit(1);
	}
	if ((fps = fopen(sal, "w")) == NULL)
	{
		fprintf(stderr, "AES-Error: no puedo abrir %s\n", sal);
		exit(1);
	}
}

void uso()
{
	printf("\nUso : $ des [-c | -d] KEY [-128 -192 -256] ARCHIVO\n");
	exit(0);
}
