//---------------------------------------------------------------------------

#ifndef AriaH
#define AriaH
//---------------------------------------------------------------------------
typedef unsigned char Byte;

void printBlock(Byte *b);
void printBlockOfLength(Byte *b, int len);

int EncKeySetup(const Byte *w0, Byte *e, int keyBits);
int DecKeySetup(const Byte *w0, Byte *d, int keyBits);
void Crypt(const Byte *p, int R, const Byte *e, Byte *c);

void ARIA_test();

#endif
