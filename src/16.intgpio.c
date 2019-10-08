
#include "STM32F4xx.h"

// FCPU =168Mhz
// FAHB =FCPU
// FAPB2=FCPU/2
// FAPB1=FCPU/4

void SystemInit()
{
unsigned int i;
    (*((int*)0xE000ED88))|=0x0F00000;  
    for (i=0;i<0x00100000;i++);     // OSC oturtma ve kurtarma rutini
    RCC->CFGR |= 0x00009400;        // AHB ve APB hizlarini max degerlere set edelim
    RCC->CR |= 0x00010000;          // HSE Xtal osc calismaya baslasin        
    while (!(RCC->CR & 0x00020000));// Xtal osc stabil hale gelsin
    RCC->PLLCFGR = 0x07402A04;      // PLL katsayilarini M=4, N=168, P=2 ve Q=7 yapalim
    RCC->CR |= 0x01000000;          // PLL calismaya baslasin  (Rehber Sayfa 95)
    while(!(RCC->CR & 0x02000000)); // Pll hazir oluncaya kadar bekle
    FLASH->ACR = 0x00000605;        // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
    RCC->CFGR |= 0x00000002;        // Sistem Clk u PLL uzerinden besleyelim
    while ((RCC->CFGR & 0x0000000F) != 0x0000000A); // Besleninceye kadar bekle
    RCC->AHB1ENR |= 0x0000000F;     // GPIO A,B,C,D clock'u aktif edelim 
    GPIOD->MODER = 0x55000000;      // GPIOD nin 15, 14, 13, 12 pinleri cikis tanimlandi (LEDler icin)
    GPIOD->OSPEEDR= 0xFFFFFFFF;     // GPIOD nin tum cikislari en yuksek hizda kullanacagiz 
    RCC->APB2ENR|=0x4000;           // SYSCFG Enable
} 

void EXTI0_IRQHandler ()
{
    EXTI->PR|=0x00000001;           // EXTIO INT flagimizi silelim
    GPIOD->ODR^=0x00001000;     
} 

int main()
{
//  Butona basinca pinimiz H oluyor

    GPIOA->MODER&=~0x00000003;      // GPIO pin ancak inp modunda interrupt uretebilir (Rehber 143)
    SYSCFG->EXTICR[0]&=~0x0000000F; // GPIO A0 interrupt uretecek
    EXTI->RTSR=1;                   // Yukselen kenar tetiklemesi yapacagiz. (EXTI0)
    EXTI->FTSR=0;                   // Dusen kenar tetiklemesi istemiyoruz.  (EXTI0)
    EXTI->IMR=1;                    // EXTI0 Int enable (Rehber 202-203)
    EXTI->EMR=0;                    // Event istemiyoruz
    NVIC->ISER[0] = 0x00000040;     // NVIC EXTI0_IRQ interrupti acalim
    while(1);      
}
