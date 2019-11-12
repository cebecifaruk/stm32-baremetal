/*
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : ENC28J60 ETH PHY Test Uygulaması
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Amaç PC programından gönderilen verinin işlenerek devre üzerindeki ledleri
 *						yakıp söndürmektir.
 * Uyari    : Keil uVision v4.70 ile derlenmiştir.
 *            EXS03 kullanıcıları;
 *            - SWDIP1 üzerindeki SW7-8 hariç tüm anahtarlar ON konumunda olmalıdır.
 *						- JP1 jumperının takılı olduğundan emin olun aksi halde ledler yanmaz.
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"

#include "exs03.h"
#include "delay.h"
#include "lcd.h"
#include "SPI.h"

#include "ENC28J60.h"
#include "net.h"
#include "ip_arp_udp_tcp.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


// LED Tanımlamaları.

#define BL0_ON GPIO_SetBits(GPIOA , GPIO_Pin_1);
#define BL1_ON GPIO_SetBits(GPIOA , GPIO_Pin_0);
#define BL2_ON GPIO_SetBits(GPIOC , GPIO_Pin_3);
#define BL3_ON GPIO_SetBits(GPIOC , GPIO_Pin_2);

#define BL0_OFF GPIO_ResetBits(GPIOA , GPIO_Pin_1);
#define BL1_OFF GPIO_ResetBits(GPIOA , GPIO_Pin_0);
#define BL2_OFF GPIO_ResetBits(GPIOC , GPIO_Pin_3);
#define BL3_OFF GPIO_ResetBits(GPIOC , GPIO_Pin_2);


#define BUFFER_SIZE 250
extern void enc28j60DumpReg(void);
static uint8_t buf[BUFFER_SIZE+1];

// Aşağıdaki satırları yerel ağınızda eşi olmayacak şekilde
// düzenleyebilirsiniz. MAC ve IP adresi başka bir cihazla
// çakışmamalıdır.

static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4] = {192,168,1,100};
static uint16_t myport =30303; // listen port for udp
static uint16_t mywwwport = 80; //tcp port

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

typedef struct udpdata
{
    uint8_t type;
    uint8_t num;
    uint8_t status;
} cmd;

/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "EXS-03 STM32F10x "
#define MESSAGE2   "Ethernet Demo   "

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

char lcdbuf[32];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    uint16_t plen;
    uint8_t payloadlen=0;
    cmd *veri;
		delay_init();	
    EXS03_initGPIO();
    GPIOC->ODR&=~(1<<8);// EXS-03 BUZZER OFF
    EXS03_initUSART();
    printf(" -   Expkits EXS-03   - \n\r");
    printf(" - STM32F10x ETH DEMO - \n\r");
    lcd_init();
    lcd_yaz(MESSAGE1);
		delay_ms(100);
    lcd_gotoxy(2,1);
    lcd_yaz(MESSAGE2);
		delay_ms(1000);
    sprintf(lcdbuf,"IP:%u.%u.%u.%u",myip[0],myip[1],myip[2],myip[3]);
    lcd_gotoxy(1,1);
    lcd_yaz(lcdbuf);
    lcd_gotoxy(2,1);
    lcd_yaz("                ");
    /* Initialize enc28j60*/
    SPI1_Init();   // ENC28J60 icin SPI i init et
    enc28j60Init(mymac);
    printf(" Chip Revision : %0X\n\r",enc28j60getrev());
    /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
    // LEDB=yellow LEDA=green
    //
    // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
    // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
    enc28j60PhyWrite(PHLCON,0x476);
    delay_ms(20);
    //init the ethernet/ip layer:
    init_ip_arp_udp_tcp(mymac,myip,mywwwport);
    /* Infinite loop */
    while (1)
    {
			
        // get the next new packet:
        plen = enc28j60PacketReceive(BUFFER_SIZE, buf);
        /*plen will ne unequal to zero if there is a valid
         * packet (without crc error) */
        if(plen==0)
        {
            continue;
        }
        // arp is broadcast if unknown but a host may also
        // verify the mac address by sending it to
        // a unicast address.
        if(eth_type_is_arp_and_my_ip(buf,plen))
        {
            make_arp_answer_from_request(buf);
            continue;
        }
        printf("Eth Type: ARP\n\r");
        // check if ip packets (icmp or udp) are for us:
        if(eth_type_is_ip_and_my_ip(buf,plen)==0)
        {
            continue;
        }
        printf("Eth Type: IP\n\r");
        if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V)
        {
            // a ping packet, let's send pong
            printf("Ping! Sending Pong!\n\r");
            make_echo_reply_from_request(buf,plen);
            continue;
        }
        if (buf[IP_PROTO_P]==IP_PROTO_UDP_V)
        {
            payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
            veri=(cmd *)&(buf[UDP_DATA_P]);
            printf("Data:%c%c%c, PayloadLen:%d\n\r",veri->type,
                   veri->num,
                   veri->status,
                   payloadlen);
					  //enc28j60DumpReg();
            switch (veri->type)
            {
            case 'L':
                switch (veri->num)
                {
                case '0':
                    switch (veri->status)
                    {
                    case '0':
                        make_udp_reply_from_request(buf,"BL00",4,myport);
                        BL0_OFF;
                        lcd_gotoxy(2,12);
                        lcd_yaz("  ");
                        break;
                    case '1':
                        make_udp_reply_from_request(buf,"BL01",4,myport);
                        BL0_ON;
                        lcd_gotoxy(2,12);
                        lcd_yaz("L0");
                        break;
                    default:
                        break;
                    }
                    break;
                case '1':
                    switch (veri->status)
                    {
                    case '0':
                        make_udp_reply_from_request(buf,"BL10",4,myport);
                        BL1_OFF;
                        lcd_gotoxy(2,9);
                        lcd_yaz("  ");
                        break;
                    case '1':
                        make_udp_reply_from_request(buf,"BL11",4,myport);
                        BL1_ON;
                        lcd_gotoxy(2,9);
                        lcd_yaz("L1");
                        break;
                    default:
                        break;
                    }
                    break;
                case '2':
                    switch (veri->status)
                    {
                    case '0':
                        make_udp_reply_from_request(buf,"BL20",4,myport);
                        BL2_OFF;
                        lcd_gotoxy(2,6);
                        lcd_yaz("  ");
                        break;
                    case '1':
                        make_udp_reply_from_request(buf,"BL21",4,myport);
                        BL2_ON;
                        lcd_gotoxy(2,6);
                        lcd_yaz("L2");
                        break;
                    default:
                        break;
                    }
                    break;
                case '3':
                    switch (veri->status)
                    {
                    case '0':
                        make_udp_reply_from_request(buf,"BL30",4,myport);
                        BL3_OFF;
                        lcd_gotoxy(2,3);
                        lcd_yaz("  ");
                        break;
                    case '1':
                        make_udp_reply_from_request(buf,"BL31",4,myport);
                        BL3_ON;
                        lcd_gotoxy(2,3);
                        lcd_yaz("L3");
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
    }
}

PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
