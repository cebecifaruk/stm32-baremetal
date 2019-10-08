#include "STM32F4xx.h"

char RxBuf[128];
char TxBuf[128];
int cntr=0;
unsigned char WAdr,RAdr;

/*********************************************************************************
CPU frekansi 168Mhz
AHB frekansi 84 Mhz
APB frekansi 42 Mhz
*********************************************************************************/

void SystemInit()
{
unsigned int i;
      (*((int*)0xE000ED88))|=0x0F00000; 
      for (i=0;i<0x00100000;i++);          // OSC oturtma ve kurtarma rutini
      RCC->CFGR |= 0x00009400;          // AHB ve APB hizlarini max degerlere set edelim
      RCC->CR |= 0x00010000;             // HSE Xtal osc calismaya baslasin       
      while (!(RCC->CR & 0x00020000)); // Xtal osc stabil hale gelsin
      RCC->PLLCFGR = 0x07402A04;      // PLL katsayilarini M=4, N=168, P=2 ve Q=7 yapalim   168 Mhz
      RCC->CR |= 0x01000000;             // PLL calismaya baslasin  (Rehber Sayfa 95)
      while(!(RCC->CR & 0x02000000));  // Pll hazir oluncaya kadar bekle
      FLASH->ACR = 0x00000605;         // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
      RCC->CFGR |= 0x00000002;          // Sistem Clk u PLL uzerinden besleyelim
      while ((RCC->CFGR & 0x0000000F) != 0x0000000A); // Besleninceye kadar bekle
      RCC->AHB1ENR |= 0x0000001F;     // GPIO A,B,C,D,E clock'u aktif edelim
      GPIOD->MODER  = 0x55550000;     // GPIOD nin 15, 14, 13, 12, 11, 10, 9, 8 pinleri cikis tanimlandi (LEDler icin)
      GPIOD->OSPEEDR= 0xFFFFFFFF;    // GPIOD nin tum cikislari en yuksek hizda kullanacagiz
}

/*********************************************************************************
      USART3 modulunu kullanarak asenkron haberlesme (Hata kontrolu yapilmiyor)
*********************************************************************************/

void USART3_IRQHandler()
{
volatile int Sts;
     Sts=USART3->SR;
     RxBuf[WAdr]=USART3->DR;
     WAdr=(WAdr+1)&0x7F;
     cntr++;
}

void UsartInit()
{
     WAdr=0;RAdr=0;

//    USART3 MODULUNU AKTIF HALE GETIRELIM

      RCC->APB1ENR|=0x00040000;    // USART3 Clk Enable (Rehber Sayfa 113)
      RCC->APB1RSTR|=0x00040000;  // USART3 Resetlendi
      GPIOB->AFR[1]=0x07777700;     // PB10..PB14 pinleri USART3 ile alakalandirildi (Hard Sayfa 49)
      GPIOB->MODER|=0x2AA00000;   // GPIOB 10..14 icin alternatif fonksiyon tanimi (Rehber Sayfa 148)

//    USART3 MODULUNU AYARLAYALIM    1 Start, 8 Data, 1 Stop, No parity (Default degerler)

      RCC->APB1RSTR&=~0x00040000; // USART3 Reseti kaldiralim
      USART3->BRR=0X1112;               // 9600 Baud

      USART3->CR1|=0x0000202C;       // USART3 enable
      USART3->CR3|=0x80;                 // DMA Transfer Enable (Transmit)
      NVIC->ISER[1]|=0x80;                // NVIC da USART3 interrupta izin verelim
}

// Rx ve TX pinlerini (GPIOB10 ve GPIOB11) birbirine baglarsaniz gonderdiginiz datalar geri gelecektir

int main()
{
volatile int i;
      UsartInit();
      RCC->AHB1ENR |= 0x00200000;        // DMA1 clock'u aktif edelim
      RCC->AHB1RSTR|= 0x00200000;        // DMA1 resetleyelim
      for(i=0;i<0x1FF;i++);         
      RCC->AHB1RSTR&=~0x00200000;        // DMA1 reseti kaldiralim
      DMA1_Stream4->M0AR=(int)&TxBuf[0]; // Nereden okuyacagiz
      DMA1_Stream4->PAR=(int)&USART3->DR;// Nereye yazacagiz
      DMA1_Stream4->FCR&=~0xFFFFFF40;
      DMA1_Stream4->CR=(DMA1_Stream4->CR & 0xF0100000)| 0x0E000440;      // CH7 vs
      for(i=0;i<128;i++) TxBuf=i+1;   // Laf olsun diye tasinacak verileri olusturalim
      DMA1_Stream4->NDTR=50;             // 50 adet veri tasiyacagiz
      USART3->SR&=~0x40;                 // Rehberde boyle yapin deniyor fakat yapmasak da oluyor !!!!
      DMA1_Stream4->CR|=1;               // Transferi baslat 

//    Asagidaki while dongusune girdigimizde DMA, Ramdaki verileri USART'dan yollarken
//    CPU da int Data received anlamina gelen interrupt ile Usartdan verileri okur
//    ve circular buffera yazar.
//    Transferin bittigini interrupt ile ya da dogrudan
//    DMA1_Stream4->CR yada  DMA_HISR registerinden ogrenebilirsiniz.
      while(1);
}

