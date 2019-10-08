
#include "STM32F4xx.h"
 
void SystemInit()
{
//  Yukarıdaki örnek programdaki kodların aynısı
}
 
void TIM7_IRQHandler()
{
volatile short i;
static char LedFlag=0;
 
    TIM7->SR=0;                                        // Timer Int Flagý silelim 
    LedFlag=(LedFlag+1)&1;
    if (LedFlag) GPIOD->ODR= 0x0000F000;     // Ledler yansin
    else GPIOD->ODR= 0x00000000;              // Ledler sonsun
} 

int main()
{
    RCC->APB1ENR|=0x00000020;         // Timer7 CLK'u aktif edelim (84 Mhz)
    TIM7->CR1=0x0080;                      // Otomatik Reload
    TIM7->PSC =42000-1;                   // Prescaler deðerimiz 42000, Count frekansimiz = fCK_PSC / (Yuklenen Deger + 1) 84E6 / (42000) = 2000 Hz
    TIM7->ARR =1000;                        // Counter, Decimal 1000 olunca basa donsun 0.5 sn demek
    TIM7->DIER=0x0001;                     // Update Int enable
    NVIC->ISER[1] = 0X00800000;        // NVIC de Timer 7 interrupta izin verelim
    TIM7->CR1|=0x0001;                    // Counter Enable
    
   while(1)
      {
      } 
}
// Programin sonu
