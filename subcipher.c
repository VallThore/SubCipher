#include "lcd.h"
#include "led.h"

/*
 * PROJEKT
 * SZYFR PODSTAWIENIOWY

 * FICZERY:
 	>wyswietlanie za pomoca przerwañ na 7-LED,
	>szyfrowanie + deszyfrowanie,
	>wyswietlanie liter,
	 opoznienie zatwierdzania liter,
	>wprowadzanie przesuniecia potencjometrem,
	>menu zawierajace opcje, przechodzenie pomiedzy opcjami za opmoca C i D, wybrana opcja miga,
	>potwiedzenie opcji za pomoca entera,
	>escape cofa do menu glownego
*/
// 0 - szyfrowanie, 1 - deszyfrowanie
static char opcja = 0;
static char etap = 0;
static unsigned long wartosc = 0;
static int wynik = 0;
static char cyfra1 = 0, cyfra2 = 0;
static char napis[17] = {0}; // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int poz = 0;
static unsigned char klawisz;
static bit wcisniety = 0;


void menu();
void odczytaj_klawisz();
void odczytaj_wartosc();
void przelicz(int result);
void odswiez_led();
void wybor_literek();
void wyswietl_napis(char *p);
void przesun(unsigned char* str, int przesuniecie);

/**
* Odswieza ekran LCD i wyswietlacz 7-led
*/
void odswiez() interrupt 1
{
	odczytaj_klawisz();
	odczytaj_wartosc();

	
}

void wyswietl_napis(char *p)
{
	static char *tekst;

	XBYTE[LCDWC] = 0x01; // clear LCD & set DD_RAM 0x00
	while(XBYTE[LCDRC] & 0x80); // czekamy az flaga BF = 0	

	tekst = p;
	while(*tekst)
	{
		if(*tekst == '\n')
		{
			XBYTE[LCDWC] = 0xC0; // przejscie do poczatku dolnej linii 1100 0000
			while(XBYTE[LCDRC] & 0x80);
			tekst++;
			continue;
		}
		XBYTE[LCDWD] = *tekst;
		tekst++;
		while(XBYTE[LCDRC] & 0x80);
	}
}

// wybieranie  literek za pomoca potencjometra
void wybor_literek()
{
	
	wyswietl_napis(napis);
	
	XBYTE[LCDWD] = 'a' + wartosc * 25/255.0;

	
}

// Odczytanie wartosci z potencjometru
void odczytaj_wartosc()
{
	XBYTE[CSAD] = 0; // start przetwazania 1 probki analogowej
	wartosc = XBYTE[CSAD]; // pobranie cyfrowego wyniku przetwazania		
}

// Odswiezanie wyswietlacza 7-SEG podczas wybierania przesuniecia
void odswiez_led()
{
	static char wej=0;
	wej++;

	if(etap == 0)
		return;

	if(wej == 1) 
		napisz(cyfra1, 2, 0);
	if(wej == 2) 
	{
		napisz(cyfra2, 1, 0);
		wej = 0;
	}

}

void przelicz(int result)
{
	wynik = result * 25/255.0 + 1;	  // przeliczanie zakresu na 1-26
	cyfra1 = wynik / 10;
	cyfra2 = (wynik % 10);
}

// Odczytywanie klawiszy z klawiatury matrycowej i podejmowanie akcji
void odczytaj_klawisz()
{
	int i;
	if (XBYTE[CSKB1] < 0xFF)
	{
		if(!wcisniety)
		{
			wcisniety = 1;
			klawisz = XBYTE[CSKB1]; 
		}
		// 1110 1111 ^
		// 1101 1111 v			
	}
	else
	{
		wcisniety = 0;
	}
	if(etap == 2)
	{
		if(klawisz == 0x7F || klawisz == 0xF7) // ENTER
		{
			napis[poz] = 'a' + wartosc * 25/255.0;
			poz++;

			if(poz == 16 || klawisz == 0xF7)
			{
				przesun(&napis, (opcja == 0 ? wynik : -wynik));
			 	etap = 3;
			}
		}
	}	
	if(etap == 1)
	{
		if(klawisz == 0x7F) // ENTER
			etap = 2;
	}
	// jezeli wybieramy opcje (szyfrowanie/deszyfrowanie)
	if(etap == 0)
	{
		if (klawisz == 0xEF) // ^ 
			opcja = 0;
		else if (klawisz == 0xDF) // v
			opcja = 1;
		else if(klawisz == 0x7F) // ENTER
			etap = 1;
	}
	
	// jezeli kazdy inny etap, przechodzimy do menu glownego
	if(etap > 0)
	{
		if(klawisz == 0xBF)	   // 1011 1111 ESC
		{
			P1_6 = 1;
			etap = 0;
			
			// zerowanie bufora wprowadzonych znakow
			for(i = 0; i < 17; i++)
			{
				napis[i] = 0;
			}
			poz = 0;
		}
	}

	klawisz = 0; // zerowanie wyboru klawisza
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
	static wait = 0;
	TMOD = 0x01; // 0000 0001 timer 0 pracujacy w trybie nr 1, czyli 16-bitowym
	EA = 1; // zezwolenie globalne na przerwanie
	ET0 = 1; // zezwolenie indywidualne dla timera0
	TH0 = 255; // punkt startowy dla okresu 50ms
	TR0 = 1; // wlaczenie timera

	// wybieranie opcji klawiszem
	XBYTE[CSMX] = 0; // wybor kanalu wejsciowego
	while(1)
	{	
		wait++;
		if(wait > 3000)
		{
			if(etap == 0) 
			{
				menu();	
			}
			else if(etap == 1)
			{ 
				przelicz(wartosc);
				wyswietl_napis("Wybierz\nprzesuniecie");
			}
			else if(etap == 2)
				wybor_literek();
			else if(etap == 3)
				wyswietl_napis(napis);
			wait = 0;
		}	

		if(etap > 0)
		{
			odswiez_led();	
		}
		else
			P1_6 = 1;
	}
}