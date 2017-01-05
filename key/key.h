#ifndef _KEY_H_
#define _KEY_H_
#include<iostm8s105c6.h>
#include<stdio.h>
#include<stdint.h>
#define KEY_1 PC_IDR_bit.IDR1
#define KEY_2 PD_IDR_bit.IDR2
#define KEY_3 PD_IDR_bit.IDR3


typedef enum
{
KEY_NUMBER_0 =0x00,
KEY_NUMBER_1,
KEY_NUMBER_2,
KEY_NUMBER_3,
KEY_NUMBER_4,
KEY_NUMBER_5,
KEY_NUMBER_6
}_KEY_NUMBER;

typedef enum
{
KEY_TYPE_NONE=0x00,
KEY_TYPE_PRESS,
KEY_TYPE_LONG_PRESS,
KEY_TYPE_HOLD,
KEY_TYPE_DOUBLE_PRESS,
KEY_TYPE_DOUBLE_LPRESS
}_KEY_TYPE;

void Key_Init();
unsigned short Key_scan(void);
unsigned char ReadKeyNumber();

#endif 