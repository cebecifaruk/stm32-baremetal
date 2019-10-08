#include "STM32F4xx.h"
 
unsigned short PWM[8];  // PWM registerler
unsigned short SRG[8];  // Shadow Registerler
unsigned short CNTR;    // PWM Counter

/*****************************************************************************************************
         CPU PLL ile 168Mhz de kosturulur
         AHB frekansy 168 Mhz
         APB1 frekansy 42 Mhz
         APB2 frekansy 84 Mhz
*****************************************************************************************************/
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
     GPIOD->MODER  = 0x55550000;     // GPIOD nin 15, 14, 13, 12, 11, 10, 9, 8 pinleri cikis tanimlandi (LEDler icin)
     GPIOD->OSPEEDR= 0xFFFFFFFF;     // GPIOD nin tum cikislari en yuksek hizda kullanacagiz 

     RCC->APB1ENR|=0x00000020;       // Timer7 CLK'u aktif edelim (84 Mhz)
     TIM7->CR1=0x0080;               // Otomatik Reload
     TIM7->PSC =839;                 // Prescaler degerimiz 839, Count frekansimiz = fCK_PSC / (Yuklenen Deger + 1) 84E6 / (840) = 100 KHz
     TIM7->ARR =1;                   // Counter, Decimal 1 olunca basa donsun. Her 20 mikrosaniye de bir timer int olusacak.
     TIM7->DIER=0x0001;              // Update Int enable
     NVIC->ISER[1] = 0X00800000;     // NVIC de Timer 7 interrupta izin verelim
     TIM7->CR1|=0x0001;              // Counter Enable
     
} 

 
void TIM7_IRQHandler()
{
unsigned short d,i,j;
 
    TIM7->SR=0;                       // Timer Int Flagini silelim 
    d=GPIOD->ODR & 0x00FF;
    CNTR++;
    if(CNTR>=1000)
      {
        CNTR=0;
        for(i=0;i<8;i++) 
         { 
           if(PWM[i]>1000) PWM[i]=1000;
           SRG[i]=PWM[i]; 
         }  
      }
    j=0x8000;
    for(i=0;i<8;i++)
      {
       if (CNTR>=SRG[i]) d|=j;
       j=j>>1;
      } 
    GPIOD->ODR=d;
} 

int main()
{
    
    PWM[0]=100;
    PWM[1]=200;
    PWM[2]=300;
    PWM[3]=400;
    PWM[4]=500;
    PWM[5]=600;
    PWM[6]=700;
    PWM[7]=800;
    
    while(1);
}
