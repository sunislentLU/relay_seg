#ifndef _BUZZER_LED_H_
#define _BUZZER_LED_H_
#include<iostm8s105c6.h>

#define LED1_ON()   PB_ODR_bit.ODR5 = 0
#define LED1_OFF()  PB_ODR_bit.ODR5 = 1

#define BUZZER_ON()   if(BEEP_CSR_bit.BEEPEN == 0)\
                       BEEP_CSR_bit.BEEPEN = 1
#define BUZZER_OFF()  if(BEEP_CSR_bit.BEEPEN == 1)\
                         BEEP_CSR_bit.BEEPEN = 0

typedef enum
{
BUZZER_OFF = 0x00,
BUZZER_ON_SHORT,
BUZZER_ON_LONG,
}_BUZZER_TYPE;

typedef enum
{
  LED_ALWAYS_OFF = 0x00,
  LED_FLASH_ONE,
  LED_FLASH_TWO,
  LED_FLASH_TRIPLE,
  LED_ALWAYS_ON,
  LED_TYPE_NONE
}_LED_TYPE;

void Buzzer_Process();
void Buzzer_Led_Init();
void led_Flash_Process();

#endif