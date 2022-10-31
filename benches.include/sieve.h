// used by both sieve routines
char * const sieveFlags = (char *) 0x6000; // 8k (0x2000) bytes for sieve table

void clearSieveData();
void clearSieveDataAsm();
