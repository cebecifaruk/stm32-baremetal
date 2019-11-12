/********************************************************************************
** Written by: ElektroEmre             
** Date: 29.01.2012           
** Version: V 1.0             
** Descriptions: E_RST , E_CS , E_INT , SCLK , SDOUT , SDIN switchleri acik olmali (digerleri kapali)
**				 Bilgisyarýn ag ayarlarindan;
**				 -- IP adresi        	192.168.1.xxx (Mesela 192.168.1.156)
**				 -- Alt Ag Maskesi 		255.255.255.0
**				 --Varsayilan Ag Gecidi	192.168.1.1
**				 olarak ayarlanamidir,
**
**				 Ethernet kablosunun bagli oldugundan emin olduktan sonra;
**				 Web tarayicisina; 192.168.1.100/888
**				 girilmeli. (IP/sifre format)
**				 Web sayfasindan kitteki ledler acilip kapatilabilir.
**				 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "systick.h"
#include "lcd.h"

#include "SPI.h"
#include "ENC28J60.h"
#include "simple_server.h"

/* Fonksiyon Prototipleri-----------------------------------------------------*/
void GPIO_Configuration(void);


/*************************Ana Program*************************/
int main(void)
{
	GPIO_Configuration();//Giris Cikilari Ayarla
	delay_init();  //Delay icin konfigurasyonlari yap
	GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
   
      /*******LED'leri AC*******/
	  GPIO_SetBits(GPIOA , GPIO_Pin_0);
	  GPIO_SetBits(GPIOA , GPIO_Pin_1);
	  GPIO_SetBits(GPIOC , GPIO_Pin_2);
	  GPIO_SetBits(GPIOC , GPIO_Pin_3);
	  delay_ms(500);
	  /******LED'leri KAPAT*****/ 
	  GPIO_ResetBits(GPIOA , GPIO_Pin_0);
	  GPIO_ResetBits(GPIOA , GPIO_Pin_1);
	  GPIO_ResetBits(GPIOC , GPIO_Pin_2);
	  GPIO_ResetBits(GPIOC , GPIO_Pin_3);
	  delay_ms(500);
	  
	lcd_init();
	lcd_yaz("    Ethernet    ");
	lcd_gotoxy(2,1);
	lcd_yaz("   Uygulamasý   ");

	SPInet_Init();   // ENC28J60 icin SPI i init et
    simple_server(); //Server
	
	while(1);		
    
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output ve PushPull
  GPIO_Init(GPIOA, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output ve PushPull
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//Output ve PushPull
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}




