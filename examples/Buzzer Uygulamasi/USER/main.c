/********************************************************************************

** Modified by: ElektroEmre             
** Modified date: 03.01.2012           
** Version: V 1.0             
** Descriptions: Tum DipSwitchler kapali konumda olmali
** 				Uygulama, degisen periyotlarda buzzer'i aktif eder.           
**
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "systick.h"
#include "lcd.h"

#define BUZZER(x) ((x) ?   GPIO_SetBits(GPIOC , GPIO_Pin_8) :  GPIO_ResetBits(GPIOC , GPIO_Pin_8) )

/* Fonksiyon Prototipleri-------------------------------------------------------*/
void GPIO_Configuration(void);
u16 kont=0;
u32 temp=0;


/*************************Ana Program*************************/
int main(void)
{
	GPIO_Configuration();//Giris Cikilari Ayarla
	delay_init();  //Delay icin konfigurasyonlari yap
	GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun


	  
	lcd_init();
	lcd_yaz("BuzzerUygulamasý");
	lcd_gotoxy(2,1);
	lcd_yaz("www.ExpKits.com");	
	delay_ms(400);
	lcd_gotoxy(2,1);
	lcd_yaz("                ");	


 	while (1)
	{

		temp+=3;
		delay_ms(temp);
		while(kont--)BUZZER(1);
		delay_ms(temp);
	 	while(kont--)BUZZER(0);
		lcd_gotoxy(2,(unsigned char) (temp/7) +1);
		lcd_yaz("*");
		
		if(temp>107)
		{
			temp=3;
			lcd_gotoxy(2,1);
			lcd_yaz("                ");

		}

	}
	
		
    
}


/*******************Giris - Cikis Pinleri Konfigurasyonlari*********************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 


  /*
  *********LED1->PA0 LED2->PA1 LED3-> PC2 LED4 -> PC3 *********
  */					 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //Input ve Pull-Down
  GPIO_Init(GPIOA, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //Input ve Pull-Down
  GPIO_Init(GPIOC, &GPIO_InitStructure);

						 
  /*
  ********* LCD PB8...PB15 *********
  */					 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output ve PushPull
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*
  *********BUZZER PC8*********
  */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 					 
					 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //Output ve PushPull
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}


