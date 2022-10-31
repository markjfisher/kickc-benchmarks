// Header for graphics dlists and constants.

#define HIBYTE(w) BYTE1(w)
#define LOBYTE(w) BYTE0(w)

char * const charset     = (char *) 0x8000;
char * const lms         = (char *) 0xa010;
char * const scoreLms    = (char *) 0xe000;
char * const fireCharset = (char *) 0xa000;
char * const fireScreen  = (char *) 0xa410;

void mode8();
void mode4();
void modeFire();
void counterRow();
void showScore();

char * strToCode(char *s);
char convertCodeToAtascii(char c);
char convertAtasciiToCode(char c);
void priorOff();
void priorOn();
void priorOffFire();
void priorOnFire();
