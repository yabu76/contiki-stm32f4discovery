#include <stm32f4xx.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_gpio.h>
#include <debug-uart.h>
#include <stdio.h>

__IO uint16_t adc_value;

void adc_init(void)
{
	// enable GPIOC and DMA2 peripheral clock
	RCC->AHB1ENR |= (1 << 2) | (1 << 22);

	// enable ADC3 clock
	RCC->APB2ENR |= (1 << 10);

	// DMA init
	DMA2_Stream0->CR = DMA_Channel_2
			| DMA_DIR_PeripheralToMemory
			| DMA_PeripheralInc_Disable
			| DMA_MemoryInc_Disable
			| DMA_PeripheralDataSize_HalfWord
			| DMA_MemoryDataSize_HalfWord
			| DMA_Mode_Circular
			| DMA_Priority_High
			| DMA_MemoryBurst_Single
			| DMA_PeripheralBurst_Single;
	DMA2_Stream0->FCR = DMA_FIFOMode_Disable
			| DMA_FIFOThreshold_HalfFull;
	DMA2_Stream0->NDTR = 1;
	DMA2_Stream0->PAR = (uint32_t)(&(ADC3->DR));
	DMA2_Stream0->M0AR = (uint32_t)&adc_value;
	DMA2_Stream0->CR |= (uint32_t)DMA_SxCR_EN;

	// enable GPIOC
	GPIOC->MODER &= ~(3 << 2 * 2);
	GPIOC->MODER |= ((uint32_t)GPIO_Mode_AN << 2 * 2);
	GPIOC->PUPDR &= ~(3 << 2 * 2);
	GPIOC->PUPDR |= ((uint32_t)GPIO_PuPd_NOPULL << 2 * 2);

	// ADC common init
	ADC->CCR = ADC_Mode_Independent
		| ADC_Prescaler_Div2
		| ADC_DMAAccessMode_Disabled
		| ADC_TwoSamplingDelay_5Cycles;

	// ADC3 init
	ADC3->CR1 |= ADC_Resolution_12b;
	ADC3->CR2 |= ADC_DataAlign_Right
		| ADC_ExternalTrigConvEdge_None
		| 1 << 1; // ADC_ContinuousConvMode ENABLE
	// number of conversion = 1
	ADC3->SQR1 &= ~(1 << 20);  

	// Configure rank in the sequencer and sample time
	ADC3->SMPR1 |= ADC_SampleTime_3Cycles << ( 3 * (ADC_Channel_12 - 10));
	ADC3->SQR3 |= ADC_Channel_12;

	// ADC3 - enable ADC DMA request after last transfer
	ADC3->CR2 |= ADC_CR2_DDS;

	// ADC3 - enable ADC DMA request
	ADC3->CR2 |= ADC_CR2_DMA;

	// ADC3 enable
	ADC3->CR2 |= ADC_CR2_ADON;

	// ADC3 - enable software conversion
	ADC3->CR2 |= ADC_CR2_SWSTART;
}

uint16_t adc_get_value()
{
	return adc_value;
}
