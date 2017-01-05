#ifndef _BUZZER_LED_H_
#define _BUZZER_LED_H_
#include<iostm8s105c6.h>

#define LED1_ON()   PB_ODR_bit.ODR5 = 0
#define LED1_OFF()  PB_ODR_bit.ODR5 = 1

#define BUZZER_ON()  PD_ODR_bit.ODR4 = 1
#define BUZZER_OFF() PD_ODR_bit.ODR4 = 0

typedef enum
{
BUZZER_OFF = 0x00,
BUZZER_ON_SHORT,
BUZZER_ON_LONG,
}_BUZZER_TYPE;
#endif


void Buzzer_Process();
void Buzzer_Led_Init();