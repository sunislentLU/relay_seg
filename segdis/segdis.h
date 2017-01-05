#ifndef _SEGDIS_H_
#define _SEGDIS_H_
#include<iostm8s105c6.h>

//段码宏定义   段码芯片HC164时钟引脚宏定义
#define     DU_CLK_Set()     PD_ODR_bit.ODR7 = 1 
#define     DU_CLK_ReSet()   PD_ODR_bit.ODR7 = 0 
//段码宏定义   段码芯片HC164数据引脚宏定义
#define     DU_DAT_Set()     PB_ODR_bit.ODR0 = 1 
#define     DU_DAT_ReSet()   PB_ODR_bit.ODR0 = 0

#define     BIT_1_ON()  PC_ODR_bit.ODR6 = 1
#define     BIT_2_ON()  PC_ODR_bit.ODR5 = 1 
#define     BIT_3_ON()  PC_ODR_bit.ODR4 = 1
#define     BIT_4_ON()  PC_ODR_bit.ODR7 = 1


#define     BIT_1_OFF()  PC_ODR_bit.ODR6 = 0
#define     BIT_2_OFF()  PC_ODR_bit.ODR5 = 0 
#define     BIT_3_OFF()  PC_ODR_bit.ODR4 = 0
#define     BIT_4_OFF()  PC_ODR_bit.ODR7 = 0

typedef struct
{
    unsigned char is_1ms;
    unsigned char is_10ms;
    unsigned char is_100ms;
    unsigned char is_5ms;
    unsigned char ms_count;
}_SEGTIME;

extern void SegDis_Init();
extern void HC164D_Write_DU(unsigned char data);
extern void HC164D_Write_WU(unsigned char data);

#endif