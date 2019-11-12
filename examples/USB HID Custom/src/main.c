/*
 * Dosya    : app.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : USB HID + ADC(LDR) Veri aktarim uygulamasi
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : ADC' den okunan veri USB HID üzerinden bilgisayara aktarılacaktir.
 *						ExpkitsUI programından ver iletişim kanalı olarak USB seçilmelidir.
 * Uyarı    : Keil uVision v4.70a ile derlenmiştir.
 *
 */

/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Custom HID demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "exs03.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "stdio.h"
#include "string.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char adc_value[32];
/* Extern variables ----------------------------------------------------------*/
__IO uint8_t PrevXferComplete = 1;
__IO uint32_t TimingDelay = 0;
extern __IO uint8_t Send_Buffer[65];
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
    volatile uint32_t i;
    unsigned int adc=0;
    GPIO_InitTypeDef GPIO_InitStructure;
    delay_init();	
    EXS03_initGPIO();//Giris Cikilari Ayarla
	  EXS03_configADC(LDR_CH);
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
    /* USB kontrol*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* USB Init */
    Set_System();
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();
    lcd_init();
    lcd_yaz("EXS03 USB HID");
    lcd_gotoxy(2,1);
    lcd_yaz("Custom Demo");
    while (1)
    {
        if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
        {
            adc=ADC_GetConversionValue(ADC1);
            adc=4095-adc;
            sprintf(adc_value,"ADC=%4u      ",adc);
            lcd_gotoxy(2,1);
            lcd_yaz(adc_value);
            for (i=0; i<0x1FFFFF; i++);
        }
        memset(&Send_Buffer,0xFF,sizeof(Send_Buffer));
        Send_Buffer[0]=adc & 0xFF;
        Send_Buffer[1]=(adc>>8) & 0x0F;
        /* Reset the control token to inform upper layer that a transfer is ongoing */
        PrevXferComplete = 0;
        /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
        USB_SIL_Write(EP1_IN, &Send_Buffer[0], 64);
        SetEPTxValid(ENDP1);
    }
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    TimingDelay = nCount;
    for(; nCount!= 0; nCount--);
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while(1)
    {
    }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
