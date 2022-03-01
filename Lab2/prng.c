#include <stdio.h>
#define SIZE 6

int bit_shift(int num, int shift) {
	return num >> shift;
}

int k_bit(int num, int k) {
	return (num>>k-1) & 1;
}

void prng(int seed) { // 1110011
	int ans, xored;
	xored = (k_bit(seed, 1) ^ k_bit(seed, 2)) ^ (k_bit(seed, 5) ^ k_bit(seed, 6));
	ans = (xored << 5) | (seed >> 1);
	printf("%d \n", ans);
	while (ans != seed){
		int xored = (k_bit(ans, 1) ^ k_bit(ans, 2)) ^ (k_bit(ans, 5) ^ k_bit(ans, 6));
		ans = (xored << 5) | (ans >> 1);
		printf("%d \n", ans);
	}
}

int main() {
	int seed;
	seed = 1;
	prng(seed);
	return 0;
}
