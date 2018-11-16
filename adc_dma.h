#ifndef _ADC_DMA_H_
#define _ADC_DMA_H_

#include "stm32f10x.h"


#define numberOfConversions 100
void ADC_DMA_init(void);
uint16_t ADC_calculateMedian(void);
#endif
//=================================================================================
