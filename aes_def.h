#define Nk  4
#define Nr  10
#define Nb  4

typedef unsigned char BYTE;
typedef unsigned int WORD;

void KeyExpansion(BYTE key [4*Nk] , WORD w[Nb*(Nr+1)] , int Nk1 );   
void RotWord(WORD *temp);
void SubBytes(BYTE *indice);
void SubWord(WORD *fourBytes);