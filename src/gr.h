// Header for graphics dlists and constants.

char * const charset     = 0x8000;
char * const lms         = 0xa010;
char * const scoreLms    = 0xe000;
char * const fireCharset = 0xa000;
char * const fireScreen  = 0xa410;

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
