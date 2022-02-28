#include <stdio.h>

int bit_shift(int num, int shift) {
	int shifted;
	shifted = num >> shift;
	return shifted;
}

int main() {
	int num, shift, ans;
	printf("Podaj liczbe: ");
	scanf("%d", &num);
	printf("Podaj o ile bitow przesunac: ");
	scanf("%d", &shift);
	ans = bit_shift(num, shift);
	printf("Wynikiem przesuniecia jest: %d", ans);
	
	return 0;
}
