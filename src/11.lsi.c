
#include "STM32F4xx.h"   
char x,y,z;   

void SystemInit()
{
unsigned int i;   
       (*((int*)0xE000ED88))|=0x0F00000; 
       for (i=0;i<0x00100000;i++);           // OSC oturtma ve kurtarma rutini
       RCC->CFGR |= 0x00009400;          // AHB ve APB hizlarini max degerlere set edelim
       RCC->CR |= 0x00010000;             // HSE Xtal osc calismaya baslasin
       while (!(RCC->CR & 0x00020000)); // Xtal osc stabil hale gelsin
       RCC->PLLCFGR = 0x07402A04;       // PLL katsayilarini M=4, N=168, P=2 ve Q=7 yapalim   168 Mhz
       RCC->CR |= 0x01000000;              // PLL calismaya baslasin  (Rehber Sayfa 95)
       while(!(RCC->CR & 0x02000000));   // Pll hazir oluncaya kadar bekle
       FLASH->ACR = 0x00000605;          // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
       RCC->CFGR |= 0x00000002;          // Sistem Clk u PLL uzerinden besleyelim
       while ((RCC->CFGR & 0x0000000F) != 0x0000000A); // Besleninceye kadar bekle
       RCC->AHB1ENR |= 0x0000001F;     // GPIO A,B,C,D,E clock'u aktif edelim
       GPIOD->MODER  = 0x55550000;     // GPIOD nin 15, 14, 13, 12, 11, 10, 9, 8 pinleri cikis tanimlandi (LEDler icin)
       GPIOD->OSPEEDR= 0xFFFFFFFF;     // GPIOD nin tum cikislari en yuksek hizda kullanacagiz

//  GPIOA da A7, A6, A5 pinleri, LIS302DL cipiyle haberlesmek icin SPI moduna alinacak   

       GPIOA->AFR[0]=0x55500000;         // SPI1 secelim (Rehber Sayfa 141), Hard Sayfa 49
       GPIOA->MODER|=0x0000A800;       // A7,A6,A5 Alternatif fonksiyon
       RCC->APB2ENR|=0x00001000;        // SPI1 clk enable   //   GPIOE3 pinini output tanimlayalim (LIS302DL SPI/I2C secimi)   
       GPIOE->MODER  = 0x00000040;      // GPIOE nin 3 nolu pini cikis tanimlandi
       GPIOE->OSPEEDR= 0xFFFFFFFF;     // GPIOE nin tum cikislari en yuksek hizda kullanacagiz
       GPIOE->BSRRL=0x0008;                // GPIOE3=1; LIS302DL CS=1   
       SPI1->CR1=0x00000B7F;               // SPI1 16 bit, master , fulldublex vs
       SPI1->CR2=0X0000;
}   

void Write(char Adr,unsigned char Data)
{
      GPIOE->BSRRH=0x0008;            // LIS302DL CS=0
      SPI1->DR=((Adr&0x3F)<<8)|Data;
      while(!(SPI1->SR&2));
      GPIOE->BSRRL=0x0008;            // LIS302DL CS=1
}   

char Read(char Adr)
{

      GPIOE->BSRRH=0x0008;            // LIS302DL CS=0
      SPI1->DR=((Adr&0x3F)|0x80)<<8;
      while(!(SPI1->SR&1));
      GPIOE->BSRRL=0x0008;            // LIS302DL CS=1
      return(SPI1->DR);
}   

int main()
{
char who;   

   if(Read(0x0F)==0x3B)   // Who are you ?
       {
          Write(0x20,0x47);  // Data Rate=100Hz, Full Scale=2g, Activate, x,y,z enable
          while(1)
            {
               who=Read(0x27); // Statusu ogrenelim. Kim hazir kim değil?
               if (who&1) x=Read(0x29);
               if (who&2) y=Read(0x2B);
               if (who&4) z=Read(0x2D);
            }
       }
      while(1);
}

