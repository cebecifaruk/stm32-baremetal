GPIO pinlerinde alternatif fonksiyonları kullanmak

Bir önceki Timer7 programında ledlerin yanıp sönme hızının beklenenden 2 kat fazla olduğu gözlenince akla gelen sorulardan birisi acaba
PLL ayarlarını yanlış yaptık da çevrebirimleri olması gerekenden 2 kat hızlı mı koşuyor oldu.

Bu programda MCO2 pininden SYSCLK sinyalini çıkartıp osiloskop ile frekansını ölçeceğiz.
Rehber Sayfa 85 de SYSCLK ve MCO2 yi inceleyin.

CPU 8 Mhz Xtal OSC ile 168Mhz de koşuyor.  (Acaba)
AHB frekansı 168 Mhz (Acaba)
APB1 frekansı 42 Mhz (Acaba)
APB2 frekansı 84 Mhz (Acaba)

// Programın başı

#include "STM32F4xx.h"

void SystemInit()
{
//  Yukarıdaki örnek programdaki kodların aynısı
}

int main()
{
//  MOC2 pininden SYSCLK cikacagiz
    RCC->CFGR &= 0x07FFFFFF;         // 31,30,29,28,27 nolu bitleri silelim (Aslinda zaten 0)  (Prescaler degerinide silelim)
    RCC->CFGR |= 0x30000000;   // MCO2 prescaler /4 secildi

//  Hard sayfa 46 dan MCO2 nin GPIO C9 pini oldugu tespit edildi
//  http://img263.imageshack.us/img263/4633/pinlistesi.jpg dan PC9 un bos pin oldugu teyid edildi
//  PC9 a hangi alternatif fonksiyon atanacağını bulalım  (Hard Sayfa 50 den MCO2 nin AF0 oldugu goruldu)

    GPIOC->AFR[1] &= ~0x000000F0;  // 7,6,5,4 nolu bitler silindi 
//  Keil yada ST yukarıdaki registeri neden structure içinde array tanımlamış anlamadım,
//  Halbuki diğer regler gibi yapabilirdi (header dosyayı değiştirmek istemiyorum ama istersek yapabiliriz)

    GPIOC->OSPEEDR =  0xFFFFFFFF;  // GPIOC nin tum cikislarinı en yuksek hizda kullanacagiz

// GPIOC de 9 nolu pine ait cikisi alternatif fonksiyon secelim

    GPIOC->MODER =  0x00080000;    // Bit 19=1 ve Bit 18=0 yaparak pinin alternatif fonksiyon olmasini sagladik


    while(1)
      {
      }
}

// Programın sonu

MCO2 pini (PC9) scopla gözlendi ve 42Mhz ölçüldü. MCO2 prescaler 4'e set edildiğindenö SYSCLK frekansımızın 4x42=168Mhz oldugu anlaşıldı.
Burda hata olsaydı yanlışlıkla overclock yapmış ve bir ihtimal sistemi 336Mhz ile besleyebilirdik.

Neden prescalerı 1 yapmadıkda 4 yaptık?  Dokümanlarda, GPIO lardan 100Mhz den yüksek frekansda sinyal çıkamazsınız deniyor.

SYSCLK 168Mhz buna rağmen Timer7 örneğindeki ledler 1 sn yerine 0.5 sn de yanıp sönüyorsa, olası durumlar;

1. APB1 BUS'ı 42 yerine 84Mhz ile besledik.
2. Timer Clk kaynağı APB1 BUS clk dan beslenmiyor.
3. Timer programında mantık hatası yapıyorum.

Sonuç: Timer7 programındaki hatayı hala bulamadık. Fakat bu program sayesinde alternatif fonksiyon kullanımını görmüş olduk. 
