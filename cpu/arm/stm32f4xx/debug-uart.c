#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_usart.h>
#include <debug-uart.h>
#include <string.h>

#ifndef DBG_UART
#define DBG_UART USART1
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
dbg_setup_uart_default()
{
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
    return 0;
}

static unsigned char dbg_write_overrun = 0;

void
dbg_putchar(const char ch)
{
  }

void
dbg_blocking_putchar(const char ch)
{
  }

void
dbg_drain()
{
}
