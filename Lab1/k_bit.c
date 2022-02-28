#include <stdio.h>

int k_bit(int num, int k) {
	int bit = (num>>k-1) & 1; // k-1 cause of starting bit has 0 index
	return bit;
}

int main() {
	int num, k, wynik;
	printf("Wprowadz liczbe: ");
	scanf("%d", &num);
	printf("Wprowadz ktory bit chcesz otrzymac: "); // now if you want bit with index no. 0 you need to insert k = 1
	scanf("%d", &k);
	wynik = k_bit(num, k);
	printf("Otrzymany bit to: %d", wynik);
	
	return 0;
}
