#define Nk  4
#define Nr  10
#define Nb  4
#define TRUE 1
#define FALSE 0

#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))   //usado para MixColumns
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))) //usado para InvMixColumns

typedef int BOOLEAN;
typedef unsigned char BYTE;
typedef unsigned int WORD;

BOOLEAN cifrar = TRUE;
//BYTE llave[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
BYTE llave[16];
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
char ent[128], sal[128];
FILE *fpe, *fps;
void CifrarDescifrarArchivo(BYTE text[16], WORD w[Nb*(Nr+1)]);
void procesa_argv();  
void uso();
