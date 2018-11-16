#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f10x.h"

void speedometerTimerInit(void);
uint8_t isCorrectHallSequence(uint8_t currentHallPosition);
void RPM_measure(void);

#endif
//EOF
