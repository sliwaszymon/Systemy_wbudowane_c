## Laboratorium nr 2
# Zadanie 1:
Generator liczb pseudolosowych. 1110011
![Tablica](tablica.jpg?raw=true "Opis zadania na tablicy")
Pobieramy bity stojące w naszym ziarnie na miejscach o indeksach 1, 2, 5 i 6. Na otrzymanych bitach wykonujemy parami operację xor (^). Wynik tej operacji przesuwamy bitowo o 5 w lewo (<<5) (gdy 1 - otrzymujemy 32, gdy 0 - otrzymujemy 0), następnie sumujemy (|) z aktualnym ziarnem przesuniętym bitowo o jeden w prawo (>>1).
