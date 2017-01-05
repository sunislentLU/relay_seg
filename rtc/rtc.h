#ifndef _RTC_H_
#define _RTC_H_
#include<iostm8s105c6.h>

//DS1302时钟芯片的时钟引脚
#define       CLK_SET()       PB_ODR_bit.ODR3 = 1
#define       CLK_RESET()     PB_ODR_bit.ODR3 = 0

//DS1302时钟芯片的数据引脚
#define       DIO_SET()       PB_ODR_bit.ODR2 = 1
#define       DIO_RESET()     PB_ODR_bit.ODR2 = 0

//DS1302时钟芯片的复位引脚
#define       RST_SET()       PD_ODR_bit.ODR0 = 1
#define       RST_RESET()     PD_ODR_bit.ODR0 = 0


#define       DIO_Set_InPut()     PB_DDR_bit.DDR2 = 0
#define       DIO_Set_OutPut()    PB_DDR_bit.DDR2 = 1
#define       DIO_Read()          PB_IDR_bit.IDR2


typedef struct 
{
 unsigned char second;
 unsigned char min;
 unsigned char hour;
 unsigned char day;
 unsigned char month;
 unsigned char week;
 unsigned int year;
}_RTC_VALUE;


extern void Rtc_Init();
extern unsigned char read_1302(unsigned char add);//从1302读数据函数，指定读取数据来源地址
extern unsigned char HEX_BCD(unsigned char hex);
extern unsigned char BCD_Decimal(unsigned char bcd);//BCD码转十进制函数，输入BCD，返回十进制
extern void DS1302SetHourMin(unsigned char hour,unsigned char min);
extern void Ds1302_Time_init(unsigned char hour , unsigned char min , unsigned char second , unsigned int year_1 ,unsigned char month ,unsigned char day ,unsigned char week);
extern void get_time();
extern unsigned char table1[7];

#endif