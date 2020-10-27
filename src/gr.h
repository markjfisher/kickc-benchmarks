// Header for graphics dlists and constants.

char * const charset    = 0x8000;
char * const lms        = 0xa010;
char * const scoreLms   = 0xe000;

void mode8();
void mode4();
void counterRow();
void showScore();

char * strToCode(char *s);
char convertCodeToAtascii(char c);
char convertAtasciiToCode(char c);
