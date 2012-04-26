#ifndef _LCD_H_
#define _LCD_H_

#include <REGX51.H>
#include <absacc.h>

#define LCDWC 0xf080 // adres bufora zapisu rozkazow sterujacych LCD
#define LCDWD 0xf081 // adres bufora zapisu kodu ASCII znaku do wyswietlenia
#define LCDRC 0xf082 // adres rejestru odczytu danych konfiguracyjnych (BF-7bit)

#define CSKB0 0xf021 // adres rejestru klawiszy 0-7
#define CSKB1 0xf022 // adres rejestru klawiszy 8-F

#endif
