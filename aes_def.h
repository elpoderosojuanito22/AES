#define Nk  4
#define Nr  10
#define Nb  4

#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

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