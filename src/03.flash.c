#include "STM32F4xx.h"

void SystemInit()
{
//  Yukarıdaki örnek programdaki kodların aynısı
}

void Delay()
{
unsigned int i;
 
     for(i=0;i<0x800000;i++); 
}

int main()
{
    while(1)
      {
         GPIOD->ODR= 0x0000F000;     // Ledler yansin
         Delay();
         GPIOD->ODR= 0x00000000;     // Ledler sonsun
         Delay();
      } 
}
// Programın sonu.
