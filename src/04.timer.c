#include "STM32F4xx.h"

void SystemInit()
{
//  Yukarıdaki örnek programdaki kodların aynısı
}

int main()
{
    RCC->APB1ENR|=0x00000020; // Timer7 CLK'u aktif edelim (42 Mhz)
    TIM7->CR1=0x0080;              // Otomatik Reload
    TIM7->PSC =42000-1;           // Prescaler değerimiz 42000, Count frekansimiz = fCK_PSC / (Yuklenen Deger + 1) 42E6 / (42000) = 1000 Hz
    TIM7->ARR =1000;                // Counter, Decimal 1000 olunca basa donsun 1 sn demek
    TIM7->CR1|=0x0001;            // Counter Enable
 
    while(1)
      {
         if (TIM7->CNT > 500) GPIOD->ODR= 0x0000F000;     // Ledler yansin
         else GPIOD->ODR= 0x00000000;                            // Ledler sonsun
      } 
}
// Programin sonu
