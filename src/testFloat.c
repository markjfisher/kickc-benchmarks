#pragma target(atarixlfloat)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <stdint.h>

uint8_t const * OUT = 0x8000;

__ma uint8_t l[1];
__ma uint8_t m[1];

void main() {
	uint8_t a[] = { 0x80, 0x4F, 0x02, 0x0D }; // 1.2345
	uint8_t b[] = { 0x80, 0x6E, 0xD9, 0xEC }; // sqrt(3) = 1.7320509
	uint32_t r;

	foo(a, b);
	foo(b, a);
}

void foo(__mem uint8_t *x1, uint8_t *x2) {
	volatile uint8_t * v1;
	uint8_t * v2;
	uint8_t a1 = 1;
	uint8_t a2 = 2;
	v1 = x1;
	v2 = &a2;

	kickasm(
		uses v1,
		uses v2
	) {{
		mov v1:v2
	}}
	
	*(OUT) = a2;
}
