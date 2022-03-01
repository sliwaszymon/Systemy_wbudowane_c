#include <stdio.h>
#define SIZE 6

int bit_shift(int num, int shift) {
	int shifted;
	shifted = num >> shift;
	return shifted;
}

int k_bit(int num, int k) {
	int bit = (num>>k-1) & 1; // k-1 cause of starting bit has 0 index
	return bit;
}

void prng(int seed) {
	int ans;
	int xored = (k_bit(seed, 1) ^ k_bit(seed, 2)) ^ (k_bit(seed, 5) ^ k_bit(seed, 6));
	ans = (xored * 32) | seed>>1;
	printf("%d \n", ans);
	while (ans != seed){
		int xored = (k_bit(ans, 1) ^ k_bit(ans, 2)) ^ (k_bit(ans, 5) ^ k_bit(ans, 6));
		ans = (xored * 32) | ans>>1;
		printf("%d \n", ans);
	}
}


int main() {
	int seed;
	seed = 1;
	prng(seed);
	return 0;
}
