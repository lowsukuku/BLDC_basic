#include "adc_dma.h"



uint16_t ADCBuffer[numberOfConversions] = {0};
uint16_t ADC_controlValue=0;

void ADC_DMA_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
    /* Enable ADC1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//							memory size 16 bit|periph size 16 bit|memory address increment|circular mode
	DMA1_Channel1->CCR|=DMA_CCR1_MSIZE_0|DMA_CCR1_PSIZE_0|DMA_CCR1_MINC|DMA_CCR1_CIRC;
	DMA1_Channel1->CMAR=(uint32_t)ADCBuffer; 		//Data is written to ADCBuffer
	DMA1_Channel1->CPAR=(uint32_t)&(ADC1->DR); 	//Data is read from ADC data register
	DMA1_Channel1->CNDTR=numberOfConversions; 	
	DMA1_Channel1->CCR|=DMA_CCR1_EN;						//Enable DMA channel
	
	ADC1->CR2|=ADC_CR2_DMA|ADC_CR2_CONT; 	//Generate DMA requests, continuous conversion mode
	ADC1->SMPR2|=ADC_SMPR2_SMP9;					//Maximum conversion time for Channel 9, less noise
	ADC1->SQR3=9;													//First conversion is Channel 9
	ADC1->CR2|=ADC_CR2_ADON;							//Enable ADC
	ADC1->CR2|=ADC_CR2_SWSTART;						//Start conversion
}

uint16_t ADC_calculateMedian(void)
{
	uint32_t sum=0;
	for (uint8_t i=0; i<numberOfConversions; i++)
	{
		sum+=ADCBuffer[i];
	}
	uint16_t median=sum/numberOfConversions;
	return median;
}
//=================================================================================
