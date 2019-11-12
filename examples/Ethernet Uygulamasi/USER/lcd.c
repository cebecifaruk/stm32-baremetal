/******************** (C) ExpKits 2011 STM32 MCU   **************************
*
* Dosya Adi          : lcd.c Header
* Yazar				 : Coskun ERGAN ( X-Fi )
* Version            : V1.0.2
* Tarih              : 25/Mart/2011
*
****************************************************************************/

#include "lcd.h"
#include "systick.h"

/***************************************************************************/
 unsigned char i,s,adres=64;
 unsigned char karakter_[8][8]=
{								/* TR Karakterler */
{ 0,14,16,16,17,14, 4, 0},//ç	 
{ 0, 0,12, 4, 4, 4,14, 0},//I
{10, 0,14,17,17,17,14, 0},//ö
{ 0,15,16,14, 1,30, 4, 0},//$
{10, 0,17,17,17,17,14, 0},//ü
								 /* özel isaretler */
{2, 6,14,30,14, 6,  2, 0},//<	 veri_yolla(05);
{ 0, 4, 4, 4, 4, 4, 4, 0},//|	 veri_yolla(06);
{ 0, 16, 8, 4, 2, 1, 0,0} //\//	 veri_yolla(07);
};
/***************************************************************************/
void lcd_busy(void)	  
{
  unsigned long h=20000;	 // delay 32Mhz = 2000

  while(h--);
}
/***************************************************************************/
void lcd_komut(unsigned char c)
{
	GPIO_Write(LCD_GPIO,((c&0xF0)<<8)|E);
	GPIO_Write(LCD_GPIO,((c&0xF0)<<8));	
    lcd_busy();

	GPIO_Write(LCD_GPIO,(c<<12)|E);	
	GPIO_Write(LCD_GPIO,(c<<12));	
    lcd_busy();
}
/***************************************************************************/
void veri_yolla(unsigned char byte)
{
switch (byte) {
case 'Ç' : byte=0x00; break; 
case 'Ý' : byte=0x01; break;
case 'Ö' : byte=0x02; break;
case 'Þ' : byte=0x03; break;
case 'Ü' : byte=0x04; break;

case 'ç' : byte=0x00; break;
case 'ý' : byte=0x01; break;
case 'ö' : byte=0x02; break;
case 'þ' : byte=0x03; break;
case 'ü' : byte=0x04; break;

default : break;
}
	GPIO_Write(LCD_GPIO,((byte&0xF0)<<8)|RS|E);
	GPIO_Write(LCD_GPIO,((byte&0xF0)<<8)|RS);	
    lcd_busy();

	GPIO_Write(LCD_GPIO,(byte<<12)|RS|E);	
	GPIO_Write(LCD_GPIO,(byte<<12)|RS);	
    lcd_busy();
}
/***************************************************************************/
void lcd_clear(void)
{
    lcd_komut(0x1);
    delay_ms(5);
}
/***************************************************************************/
void lcd_yaz(const char *s)
{
	while(*s)
		veri_yolla(*s++);
}
/***************************************************************************/
void lcd_gotoxy(unsigned char x,unsigned char y)
{
    if(x==1)
    lcd_komut(0x80+((y-1)%16));
    else
	lcd_komut(0xC0+((y-1)%16));
}
/***************************************************************************/
void lcd_enable(void)
{
    GPIO_Write(LCD_GPIO,E);
    lcd_busy();
	GPIO_Write(LCD_GPIO,0x0);
}
/***************************************************************************/	
/***************************************************************************/
/***************************************************************************/
void lcd_init()
{
    GPIO_Write(LCD_GPIO,0x0000);
	
    delay_ms(100);
    lcd_komut(0x03);
    delay_ms(1);
    lcd_enable();
    delay_ms(1);
    lcd_enable();
    lcd_komut(0x02);
    delay_ms(1);
 	lcd_komut(0x33);
    lcd_komut(0x32);
    lcd_komut(0x28);
    lcd_komut(0x0C);

for(i=0;i<=7;i++){  // tr karakter tanitiliyor
	lcd_komut(adres);
	for(s=0;s<=7;s++){
		veri_yolla(karakter_[i][s]);
					 }				  
		adres=adres+8;	 
				 }

    lcd_clear();
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
