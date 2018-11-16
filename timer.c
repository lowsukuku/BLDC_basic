#include "timer.h"

#define coreClockFrequencykHz 72000
#define timerFrequencykHz 100
#define hallPulsesPerRotation 90

float motorRPM=0;
uint8_t previousHallPosition=0;

void speedometerTimerInit(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM4EN;
	TIM4->CR1|=TIM_CR1_URS;
	TIM4->EGR|=TIM_EGR_UG;
	TIM4->SR&=~TIM_SR_UIF;
	NVIC_SetPriority(TIM4_IRQn,0);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4->DIER|=TIM_DIER_UIE;
	TIM4->PSC=coreClockFrequencykHz/timerFrequencykHz-1;
	TIM4->ARR=0xFFFF;
}

void TIM4_IRQHandler(void)
{
	TIM4->SR&=~TIM_SR_UIF;
	motorRPM=0;
}

uint8_t isPowerOfTwo(uint8_t number)
{
	return !(number&(number-1));
}

uint8_t isCorrectHallSequence(uint8_t currentHallPosition)
{
	if (previousHallPosition==0&&currentHallPosition!=0&&currentHallPosition!=7)
	{
		previousHallPosition=currentHallPosition;
		return 0;
	}		
	uint8_t changedBits=currentHallPosition^previousHallPosition; //xor for changed bits check	
	uint8_t isOneBitChanged=isPowerOfTwo(changedBits); //if 1 bit changed, it is power of two
	if (isOneBitChanged) previousHallPosition=currentHallPosition;
	return isOneBitChanged;
}

void RPM_measure(void)
{
	uint16_t ticksPassed=TIM4->CNT;
	TIM4->EGR|=TIM_EGR_UG;
	float secondsPassed=ticksPassed/(timerFrequencykHz*1000);
	float rotationsPerSecond=(1/secondsPassed)/hallPulsesPerRotation;
	motorRPM=rotationsPerSecond*60;
}

//EOF