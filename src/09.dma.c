#include "STM32F4xx.h" 
 
char Source[1024];
char Target[1024];
 
void SystemInit()
{
unsigned int i;

    (*((int*)0xE000ED88))|=0x0F00000;  
    for (i=0;i<0x00100000;i++);       // OSC oturtma ve kurtarma rutini
    RCC->CFGR |= 0x00009400;          // AHB ve APB hizlarini max degerlere set edelim
    RCC->CR |= 0x00010000;            // HSE Xtal osc calismaya baslasin        
    while (!(RCC->CR & 0x00020000));  // Xtal osc stabil hale gelsin
    RCC->PLLCFGR = 0x07402584;        // PLL katsayilarini M=4, N=150, P=2 ve Q=7 yapalim   150 Mhz 
    RCC->CR |= 0x01000000;            // PLL calismaya baslasin  (Rehber Sayfa 95)
    while(!(RCC->CR & 0x02000000));   // Pll hazir oluncaya kadar bekle
    FLASH->ACR = 0x00000605;          // Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
    RCC->CFGR |= 0x00000002;          // Sistem Clk u PLL uzerinden besleyelim
    while ((RCC->CFGR & 0x0000000F) != 0x0000000A); // Besleninceye kadar bekle
    RCC->AHB1ENR |= 0x0000000F;       // GPIO A,B,C,D clock'u aktif edelim 
    GPIOD->MODER  = 0x55550000;       // GPIOD nin 15, 14, 13, 12, 11, 10, 9, 8 pinleri cikis tanimlandi (LEDler icin)
    GPIOD->OSPEEDR= 0xFFFFFFFF;       // GPIOD nin tum cikislari en yuksek hizda kullanacagiz 
} 
 

int main()
{
volatile int i;
 
    RCC->AHB1ENR |= 0x00400000;       // DMA2 clock'u aktif edelim 
 
    while(DMA2_Stream0->CR&1);    // DMA 0 kanalinin isi varsa bitmesini bekleyelim
    DMA2_Stream0->PAR=(int)&Source[0];  // Source array dan datalari alacagiz
    DMA2_Stream0->M0AR=(int)&Target[0];  // Target array a datalari tasiyacagiz
    DMA2_Stream0->NDTR=1024;    // 1024 adet veri tasiyacagiz
    DMA2_Stream0->CR=0x00A00680;
 
    for(i=0;i<1024;i++) Source[i]=i;  // Source alanina datalarimizi koyalim
 
    DMA2_Stream0->CR|=1;     // Transferi baslat
 
// Artik biz kendi yolumuza DMA kendi yoluna 
 
   for(i=0;i<0x100000;i++);
 
    while(DMA2_Stream0->CR&1);    // DMA2 0 kanalinin isini bitirmediyse bekleyelim
      {
      }   
}
