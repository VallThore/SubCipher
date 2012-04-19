/*
 * PROJEKT
 * SZYFR PRZESTAWIENIOWY

 * FICZERY:
 	wyswietlanie za pomoca przerwañ na 7-LED,
	szyfrowanie + deszyfrowanie,
	wprowadzanie przesuniecia potencjometrem,
	menu zawierajace opcje, przechodzenie pomiedzy opcjami za opmoca A i B, wybrana opcja miga,
		potwiedzenie opcji za pomoca entera,
	escape cofa do menu glownego
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

void main()
{
	
	while(1);
}