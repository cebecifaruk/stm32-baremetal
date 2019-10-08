Timer 7 Programındaki hatanın sebebi

Bir önceki Timer7 programında ledlerin yanıp sönme hızının beklenenden 2 kat fazla olduğunu gözleyince Timer7 nin CLK frekansını 2 kat yüksek olmasına neden olacak BUS frekans hatta CPU CLK (SYSCLK) ayarlamalarını yanlıs yaptığımdan şüphelenmiştim. Ancak MCO2 pininden SYSCLK sinyalini gözetledik ve frekansının doğru şekilde 168Mhz olduğunu ispatladık. Geriye http://www.cncdesigner.com/STM/Chip.JPG den TIMER7 nin bağlı olduğu APB1 BUS'ının iki kat yüksek frekansla beslendiği ihtimali kaldı. Fakat SystemInit kodlarında prescaler değerlerinin de doğru olduğu anlaşıldı. Bunun üzerine Rehber dokumanı karıştırırken Sayfa 86 daki şu açıklama dikkatimi çekti.

The timer clock frequencies are automatically set by hardware. There are two cases:
1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as that of the APB domain to which the timers are connected.
2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
timers are connected.

Neticede diyorki;

Eğer APB prescaler 1 değilse Timerlar bağlı oluğu APB BUS frekansından 2 kat yüksek frekansla saydırılır.
(APB1 için prescaler değerimiz 4 dü. O halde Timer7 ye 42 değil 84Mhz giriyor.)

Bu durumda ledlerimiz neden 1 sn yerine 0.5 saniyede yanıp sönüyor anlaşılmış oldu. Sözkonusu durumu artık aşağıdaki gibi çözeceğiz.

Timer 7 kullanarak Ledleri flash eden program (Interrupt kullanılmıyor)

CPU 8 Mhz Xtal OSC ile 168Mhz de koşuyor. 
AHB frekansı 168 Mhz
APB1 frekansı 42 Mhz
APB2 frekansı 84 Mhz
Timer7 içeriği gözlenerek ledler yakılıp söndürülüyor

// Programın başı

#include "STM32F4xx.h"

void SystemInit()
{
//  Yukarıdaki örnek programdaki kodların aynısı
}

int main()
{
    RCC->APB1ENR|=0x00000020; // Timer7 CLK'u aktif edelim (84 Mhz)
    TIM7->CR1=0x0080;                 // Otomatik Reload
    TIM7->PSC =42000;                 // Prescaler değerimiz 42000, Count frekansimiz = fCK_PSC / (Yuklenen Deger + 1) 84E6 / (42000) = 2000 Hz
    TIM7->ARR =2000;                   // Counter, Decimal 2000 olunca basa donsun 1 sn demek
    TIM7->CR1|=0x0001;                // Counter Enable

    while(1)
      {
         if (TIM7->CNT > 1000) GPIOD->ODR= 0x0000F000;     // Ledler yansin
         else GPIOD->ODR= 0x00000000;                            // Ledler sonsun
      }
}
// Programin sonu
