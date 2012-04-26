#include "led.h"
#include "lcd.h"
/*
 * PROJEKT
 * SZYFR PODSTAWIENIOWY

 * FICZERY:
 	wyswietlanie za pomoca przerwañ na 7-LED,
	>szyfrowanie + deszyfrowanie,
	wprowadzanie przesuniecia potencjometrem,
	menu zawierajace opcje, przechodzenie pomiedzy opcjami za opmoca A i B, wybrana opcja miga,
		potwiedzenie opcji za pomoca entera,
	escape cofa do menu glownego
*/
static char opcja = 0;

void menu();
void odczytaj_klawisz();
static unsigned char klawisz;

/**
* Odswieza ekran LCD i wyswietlacz 7-led
*/
void odswiez() interrupt 1
{
	odczytaj_klawisz();
	menu();
}

void odczytaj_klawisz()
{
	if (XBYTE[CSKB1] < 0xFF)
	{
		klawisz = XBYTE[CSKB1]; 
		// 1110 1111 ^
		// 1101 1111 v			
	}
}

/**
* Funkcja szyfrujaca i deszyfrujaca
* przedsuwa znak o zadana ilosc miejsc
*/
void przesun(unsigned char* str, int przesuniecie)
{
	if(przesuniecie < 0) // deszyfrownanie
			przesuniecie = 26 + przesuniecie;

	while(*str != '\0')	// przesuwa do konca stringa
	{
		// przesuwanie znaku o zadana wartosc
		*str -= 'a';
		*str += przesuniecie;
		*str %= 26;
		*str += 'a'; 

		str++;
	}
}

/**
* Funcja wyswietla menu
*/
void menu()
{
	static char *opcje[] = {"1. Szyfrowanie", "2. Deszyfrowanie"}; // !!!
	static char *tekst;
	static int licznik = 0;

	if (klawisz == 0xEF) 
		opcja = 0;
	if (klawisz == 0xDF)
		opcja = 1;

	klawisz = 0; // zerowanie wyboru klawisza

	XBYTE[LCDWC] = 0x01; // clear LCD & set DD_RAM 0x00
	while(XBYTE[LCDRC] & 0x80); // czekamy az flaga BF = 0



		if(opcja == 1 || licznik < 10)
		{
			tekst = opcje[0];
		
			while(*tekst)
			{
				XBYTE[LCDWD] = *tekst;
				tekst++;
				while(XBYTE[LCDRC] & 0x80);
			}
			
		}
		
		if(opcja == 0 || licznik < 10)
		{
			XBYTE[LCDWC] = 0xC0; // przejscie do poczatku dolnej linii 1100 0000
			while(XBYTE[LCDRC] & 0x80);
	
			tekst = opcje[1];
			while(*tekst)
			{
				XBYTE[LCDWD] = *tekst;
				tekst++;
				while(XBYTE[LCDRC] & 0x80);
			}
		}

		if(licznik++ >= 20)
				licznik = 0;
	
}

void main()
{
	TMOD = 0x01; // 0000 0001 timer 0 pracujacy w trybie nr 1, czyli 16-bitowym
	EA = 1; // zezwolenie globalne na przerwanie
	ET0 = 1; // zezwolenie indywidualne dla timera0
	TH0 = 76; // punkt startowy dla okresu 50ms
	TR0 = 1; // wlaczenie timera

	// wybieranie opcji klawiszem
	
	while(1) ;
}