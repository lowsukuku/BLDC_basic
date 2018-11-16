#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "string.h"
#include "stdio.h"

#include "sysclk.h"
#include "adc_dma.h"
#include "bldc.h"

extern uint16_t ADC_controlValue;

int main(void)
{
	// Set clock
	SetSysClockTo72();
	// ADC Init
	ADC_DMA_init();
	// TIM1, outputs, inputs, interrupts, etc. Init
	BLDC_Init();

	// Reverse pin Init
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);

    while(1)
    {
			ADC_controlValue=ADC_calculateMedian();
			
    	if (ADC_controlValue > BLDC_ADC_START) {
    		if (BLDC_MotorGetSpin() == BLDC_STOP) {
    			// Check Reverse pin
    			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) != 0) {
    				// Forward
    				BLDC_MotorSetSpin(BLDC_CW);
    			}
    			else {
    				// Backward
    				BLDC_MotorSetSpin(BLDC_CCW);
    			}
    			BLDC_MotorCommutation(BLDC_HallSensorsGetPosition());
    		}
 
            
            BLDC_SetPWM(BLDC_ADCToPWM(ADC_controlValue));
    	}
    	else {
    		if (BLDC_MotorGetSpin() != BLDC_STOP) {
    			if (ADC_controlValue < BLDC_ADC_STOP) {
    				BLDC_MotorStop();
    			}
    		}
    	}
    }
}
