#pragma code_seg(Code)

// 8 bit converted md5 calculator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void runMd5() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("MD5 512B 5x               \x1d");

	counterOn(1);
	benchmarkMd5();
	counterOn(0);

	waitFrames(10);
	counterPrint();
}

void benchmarkMd5() {
	uint8_t const * data = 0x6000;
	for (uint16_t i = 0; i < 512; i++) { data[i] = (uint8_t) i; }
	for (uint8_t l: 0..4) { md5(data, 512); } 
}

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

uint32_t h0;
uint32_t h1;
uint32_t h2;
uint32_t h3;

inline uint16_t mul3(uint8_t a) {
	return ((uint16_t) a) * 3;
}

inline uint16_t mul5(uint8_t a) {
	return ((uint16_t) a) * 5;
}

inline uint16_t mul7(uint8_t a) {
	return ((uint16_t) a) * 7;
}

inline uint8_t mod16(uint16_t a) {
	uint16_t t = a % 16;
	return (uint8_t) (t & 0xff);
}

void md5(uint8_t *initial_msg, size_t initial_len) {
	uint8_t *msg;
	
	uint8_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
	
	uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
 
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
    
    uint16_t new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;

    msg = calloc(new_len + 64, 1);
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 128;
    
    uint32_t bits_len = initial_len * 8;
    memcpy(msg + new_len, &bits_len, 4);
    
    for(int offset=0; offset<new_len; offset += (512/8)) {
		uint32_t *w = (uint32_t *) (msg + offset);
		uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        
        for(uint8_t i: 0..63) {
			uint32_t f;
			uint8_t g;
			switch ((i >> 4) & 3)
            {
                case 0:    // (i < 16)
                    f = (b & c) | ((~b) & d);
                    g = i;
                    break;
                case 1:    // (i < 32)
                    f = (d & b) | ((~d) & c);
                    g = mod16(mul5(i) + 1);
                    break;
                case 2:    // (i < 48)
                    f = b ^ c ^ d;
                    g = mod16(mul3(i) + 5);
                    break;
                case 3:    // other
                    f = c ^ (b | (~d));
                    g = mod16(mul7(i));
                    break;
            }

			uint32_t temp = d;
            d = c + 0;
            c = b + 0;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);;
            a = temp;
		}
		
		h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
	}
}
