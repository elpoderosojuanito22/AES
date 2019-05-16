#define Nk  4
#define Nr  10
#define Nb  4

#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))   //usado para MixColumns
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))) //usado para InvMixColumns

typedef unsigned char BYTE;
typedef unsigned int WORD;

void KeyExpansion(BYTE key [4*Nk] , WORD w[Nb*(Nr+1)] , int Nk1 );   
void RotWord(WORD *temp);
void SubBytes(BYTE *indice);
void SubWord(WORD *fourBytes);
void Cipher (BYTE in[4*Nb], BYTE out[4*Nb], WORD w[Nb*(Nr+1)]);
void AddRoundKey(BYTE state[4][Nb], WORD w[] , int inicio , int termino);
void ShiftRows(BYTE state[4][4]);
void MixColumns(BYTE state[4][4]);
/*Funciones utlizadas para el descifrado*/

void InvCipher (BYTE in[4*Nb], BYTE out[4*Nb], WORD w[Nb*(Nr+1)]);
void InvSubBytes(BYTE *indice);
void InvShiftRows(BYTE state[4][Nb]);
void InvMixColumns(BYTE state[4][Nb]);

/*Funciones para el manejo de archivos*/
FILE *fpe, *fps;
void leer(BYTE text[16], WORD w[Nb*(Nr+1)]);
