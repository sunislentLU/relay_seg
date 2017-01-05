#ifndef _RELAY_H_
#define _RELAY_H_
#include<iostm8s105c6.h>
#define RELAY_1_ON()  PC_ODR_bit.ODR2 = 1
#define RELAY_2_ON()  PC_ODR_bit.ODR3 = 1
#define RELAY_3_ON()  PE_ODR_bit.ODR5 = 1

#define RELAY_1_OFF()  PC_ODR_bit.ODR2 = 0
#define RELAY_2_OFF()  PC_ODR_bit.ODR3 = 0
#define RELAY_3_OFF()  PE_ODR_bit.ODR5 = 0

void Relay_Init();
void Relay_1_On(void);
void Relay_2_On(void);
void Relay_3_On(void);
void Relay_1_Off(void);
void Relay_2_Off(void);
void Relay_3_Off(void);
#endif