/********************************************************************************

** Modified by: ElektroEmre             
** Modified date: 03.01.2012           
** Version: V 1.0             
** Descriptions: *Tum DipSwitchler kapali konumda olmali,
**				 *LCD'nin kontrast potunu kontrol ediniz.
**				 *DS18B20'nin dogru takili oldugundan emin olunuz.
**				 -DS18B20'den okunan sicakligi ekranda gosterir.           
**
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "systick.h"
#include "lcd.h"
#include "ds18b20.h"
#include "stdio.h"

float sicaklik;
char sicaklik_bilgisi[16]="";
static vu32 TimingDelay;

/* Fonksiyon Prototipleri-------------------------------------------------------*/
void GPIO_Configuration(void);



/*************************Ana Program*************************/
int main(void)
{
	GPIO_Configuration();//Giris Cikilari Ayarla
	delay_init();  //Delay icin konfigurasyonlari yap
	GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun


	  
	lcd_init();
	lcd_yaz("    DS18B20     ");
	lcd_gotoxy(2,1);
	lcd_yaz("www.ExpKits.com ");	
	delay_ms(1500);
	lcd_gotoxy(2,1);
	lcd_yaz("                ");	


 while (1)
	{
	sicaklik=ds18b20_read();
	if(sicaklik>4095){sprintf(sicaklik_bilgisi,"   Sensör Yok   ");}
	else{ sprintf(sicaklik_bilgisi,"  Isý: %3.1f", sicaklik);}

 	lcd_gotoxy(2,1);
	lcd_yaz(sicaklik_bilgisi);
 	veri_yolla(5);
    lcd_yaz("C   ");
		
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
  *********SW3->PA0 SW2->PA1 SW5-> PC2 SW4 -> PC3 *********
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output ve PushPull 
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}








#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
