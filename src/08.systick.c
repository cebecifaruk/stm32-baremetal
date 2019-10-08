#include "STM32F4xx.h"

#define STK_CTRL (*((volatile unsigned int*) 0xE000E010)) 
#define STK_LOAD (*((volatile unsigned int*) 0xE000E014)) 
#define STK_VAL   (*((volatile unsigned int*) 0xE000E018)) 

/*****************************************************************************************************
         CPU PLL ile 168Mhz de kosturulur
         AHB frekansý 168 Mhz
         APB1 frekansý 42 Mhz
         APB2 frekansý 84 Mhz
*****************************************************************************************************/
void SystemInit()
{
unsigned int i;

     (*((int*)0xE000ED88))|=0x0F00000; 
     for (i=0;i<0x00100000;i++);     // OSC oturtma ve kurtarma rutini
     RCC->CFGR |= 0x00009400;        // AHB ve APB hizlarini max degerlere set edelim
     RCC->CR |= 0x00010000;          // HSE Xtal osc calismaya baslasin       
     while (!(RCC->CR & 0x00020000));// Xtal osc stabil hale gelsin
//   RCC->PLLCFGR = 0x07405408;      // PLL katsayilarini M=8, N=336, P=2 ve Q=7 yapalim
     RCC->PLLCFGR = 0x07402A04;      // PLL katsayilarini M=4, N=168, P=2 ve Q=7 yapalim
     RCC->CR |= 0x01000000;          // PLL calismaya baslasin  (Rehber Sayfa 95)
     while(!(RCC->CR & 0x02000000)); // Pll hazir oluncaya kadar bekle
//   FLASH->ACR = 0x00000705;        // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
     FLASH->ACR = 0x00000605;        // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
     RCC->CFGR |= 0x00000002;        // Sistem Clk u PLL uzerinden besleyelim
     while ((RCC->CFGR & 0x0000000F) != 0x0000000A); // Besleninceye kadar bekle
     RCC->AHB1ENR |= 0x0000000F;     // GPIO A,B,C,D clock'u aktif edelim
     GPIOD->MODER = 0x55000000;      // GPIOD nin 15, 14, 13, 12 pinleri cikis tanimlandi (LEDler icin)
     GPIOD->OSPEEDR= 0xFFFFFFFF;     // GPIOD nin tum cikislari en yuksek hizda kullanacagiz
}

/*****************************************************************************************************
     STS TICK COUNTER AHB FREKANSINDA (168 MHZ) KOSACAK
*****************************************************************************************************/
void SysTickInit()
{
     STK_LOAD= 0X00029040>>1; // 168E6/1000/2=168000 TICK TIME 1MS
     STK_CTRL&=~0x00010000;   // Count Flagi silelim,
     STK_CTRL|= 0x00000007;   // int enb ve count enb yapalim, clk kaynagini AHB yapalim 
}

void SysTick_Handler()
{
volatile static int flag=0;
     STK_CTRL&= ~0x00010000; // Count Flagi silelim
     flag^=0xF000;
     GPIOD->ODR= flag;       // LEDLER yansin
}

int main()
{
     SysTickInit();
     while(1)
        {
   
        }
}

