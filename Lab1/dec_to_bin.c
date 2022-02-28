#include <stdio.h>
#define INT_SIZE sizeof(int) * 8

void dec_to_bin(int *bin, int dec) {
	int index;
	index = INT_SIZE - 1;
	while(index >= 0) {
		bin[index] = dec & 1;
		index--;
		dec >>= 1;
	}
}

int main() {
   int bin[INT_SIZE];
   int dec, i;
   printf("Wprowadz liczbe: ");
   scanf("%d", &dec);
   dec_to_bin(&bin[0], dec); // first index given as an arg
   for (i = 0; i < INT_SIZE; i++) {
		printf("%d", bin[i]);
   }
   return 0;
}
