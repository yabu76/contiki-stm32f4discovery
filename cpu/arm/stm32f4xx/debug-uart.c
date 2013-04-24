#include <contiki-conf.h>
#include "contiki.h"
#include <stm32f4xx_conf.h>
#include <stm32f4xx.h>
#include <core_cm4.h>
#include <debug-uart.h>
#include <string.h>

#ifndef DBG_UART
#define DBG_UART USART2
#endif

#ifndef DBG_DMA_NO
#define DBG_DMA_NO 1
#endif

#ifndef DBG_DMA_CHANNEL_NO
#define DBG_DMA_CHANNEL_NO 4
#endif


#define _DBG_DMA_NAME(x) DMA##x
#define DBG_DMA_NAME(x) _DBG_DMA_NAME(x)
#define DBG_DMA DBG_DMA_NAME(DBG_DMA_NO)

#define _DMA_CHANNEL_NAME(x,c) DMA ## x ## _Channel ## c
#define DMA_CHANNEL_NAME(x,c) _DMA_CHANNEL_NAME(x,c)
#define DBG_DMA_CHANNEL  DMA_CHANNEL_NAME(DBG_DMA_NO, DBG_DMA_CHANNEL_NO)

#define _DBG_DMA_CHANNEL_IFCR_CGIF(c) DMA_IFCR_CGIF ## c
#define _XDBG_DMA_CHANNEL_IFCR_CGIF(c) _DBG_DMA_CHANNEL_IFCR_CGIF(c)
#define DBG_DMA_CHANNEL_IFCR_CGIF \
_XDBG_DMA_CHANNEL_IFCR_CGIF(DBG_DMA_CHANNEL_NO)


#ifndef DBG_XMIT_BUFFER_LEN
#define DBG_XMIT_BUFFER_LEN 1024
#endif

static unsigned char xmit_buffer[DBG_XMIT_BUFFER_LEN];
#define XMIT_BUFFER_END &xmit_buffer[DBG_XMIT_BUFFER_LEN]

void
dbg_setup_uart()
{
    uint32_t apbclock = 21000000;
    uint32_t baud = 9600;
    uint32_t tmpreg = 0x00;
    uint32_t integerdivider = 0x00;
    uint32_t fractionaldivider = 0x00;

    RCC->APB1ENR = (1 << 17); /* enable clock for USART2 */
    RCC->AHB1ENR = (1 << 3);  /* enable clock for GPIOD */
    GPIOD->MODER |= (2 << 10); 
    GPIOD->MODER |= (2 << 12);
    GPIOD->OTYPER &= ~(3 << 10);
    GPIOD->OTYPER &= ~(3 << 12);
    GPIOD->OSPEEDR |= (2 << 10);
    GPIOD->OSPEEDR |= (2 << 12);
    GPIOD->PUPDR &= ~(3 << 10);
    GPIOD->PUPDR &= ~(3 << 12);
    GPIOD->AFR[0] |= (7 << 20);
    GPIOD->AFR[0] |= (7 << 24);
    USART2->CR1 |= (1 << 13);

    integerdivider = ((25 * apbclock) / (2 * baud));
    tmpreg = (integerdivider / 100) << 4;
    fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
    USART2->BRR = (uint16_t)tmpreg;
    USART2->CR1 |= (3 << 2);
}

/* Valid data in head to tail-1 */
/* Read position */
static unsigned char * volatile xmit_buffer_head = xmit_buffer;

/* Write position */
static unsigned char * volatile xmit_buffer_tail = xmit_buffer;

/* xmit_buffer_head == xmit_buffer_tail means empty so we can only store
   DBG_XMIT_BUFFER_LEN-1 characters */

volatile unsigned char dma_running = 0;
static unsigned char * volatile dma_end;
void
DMA1_Channel4_handler() __attribute__((interrupt));


static void
update_dma(void)
{
 }

unsigned int
dbg_send_bytes(const unsigned char *seq, unsigned int len)
{
    int i = 0;

   while (i < len)
   {
       dbg_putchar(seq[i++]);
   } 

   return i;
}

static unsigned char dbg_write_overrun = 0;

void
dbg_putchar(char ch)
{
    while (((USART2->SR >> 6) & 0x01) == 0);
    USART2->DR = (uint8_t)ch;
}

void
dbg_blocking_putchar(const char ch)
{
    dbg_putchar(ch);
  }

void
dbg_drain()
{
}
