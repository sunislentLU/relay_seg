#include<segdis.h>
#include<main.h>

//数码管0 ~ F的代码数组
unsigned char const Data[18] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0x00,0xff};
unsigned char const Wu[4] = {0x01,0x02,0x03,0x04};
/*******************************************************************************
**函数名称：void HC164D_Write_DU(unsigned char data)
**功能描述：向段码芯片HC164D写段码数据
**入口参数：unsigned char data
**输出：无
*******************************************************************************/

void SegDis_Init()
{
  //HC164芯片 位码时钟引脚初始化
  PB_DDR_bit.DDR0 = 1;  //设置端口PB->0的输入输出方向寄存器为输出方向
  PB_CR1_bit.C10  = 1;  //设置为推挽输出
  PB_CR2_bit.C20  = 1;  //设置输出速率为10MHZ
  
  PD_DDR_bit.DDR7 = 1;  //设置端口PD->7的输入输出方向寄存器为输出方向
  PD_CR1_bit.C17  = 1;  //设置为推挽输出
  PD_CR2_bit.C27  = 1;  //设置输出速率为10MHZ
  
  
  //HC164芯片 位码数码引脚初始化
  PC_DDR_bit.DDR6 = 1;  //设置端口PA->4的输入输出方向寄存器为输出方向
  PC_CR1_bit.C16  = 1;  //设置为推挽输出
  PC_CR2_bit.C26  = 1;  //设置输出速率为10MHZ
  //HC164芯片 段码时钟引脚初始化
  PC_DDR_bit.DDR7 = 1;  //设置端口PA->5的输入输出方向寄存器为输出方向
  PC_CR1_bit.C17  = 1;  //设置为推挽输出
  PC_CR2_bit.C27  = 1;  //设置输出速率为10MHZ
  //HC164芯片 段码数据引脚初始化
  PC_DDR_bit.DDR4 = 1;  //设置端口PA->6的输入输出方向寄存器为输出方向
  PC_CR1_bit.C14  = 1;  //设置为推挽输出
  PC_CR2_bit.C24  = 1;  //设置输出速率为10MHZ
  
  PC_DDR_bit.DDR5 = 1;  //设置端口PA->6的输入输出方向寄存器为输出方向
  PC_CR1_bit.C15  = 1;  //设置为推挽输出
  PC_CR2_bit.C25  = 1;  //设置输出速率为10MHZ
  
}

void HC164D_Write_DU(unsigned char data)
{
  unsigned char i;
  
  for(i = 0 ; i < 8 ; i++)
  {
    DU_CLK_ReSet();
    if(data & 0x80)
    {
      DU_DAT_Set();
    }
    else
    {
      DU_DAT_ReSet();
    }
    DU_CLK_Set(); 
    data <<= 1;
  }
  DU_CLK_ReSet();
}
/*******************************************************************************
**函数名称：void HC164D_Write_WU(unsigned char data)
**功能描述：向位码芯片HC164D写位码数据
**入口参数：unsigned char data
**输出：无
*******************************************************************************/
void AllBitOff()
{
  BIT_1_OFF();
  BIT_2_OFF();
  BIT_3_OFF();
  BIT_4_OFF();
}
void HC164D_Write_WU(unsigned char data)
{
  AllBitOff();
  switch(data)
  {
  case 0:
    AllBitOff();
    break;
  case 1:
    BIT_1_ON();
    break;
  case 2:
    BIT_2_ON();
    break;
  case 3:
    BIT_3_ON();
    break;
  case 4:
    BIT_4_ON();
    break;
  default:
    break;
    
  }
}
/*******************************************************************************
**函数名称：void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**功能描述：大概延时
**入口参数：unsigned int ms   输入大概延时数值
**输出：无
*******************************************************************************/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  通过一定周期循环进行延时*/
    for(y = 100 ; y > 0 ; y--);
}


unsigned char is_1ms = 0;
unsigned char is_10ms = 0;
unsigned char is_100ms = 0;
unsigned char is_5ms = 0;
unsigned char ms_count=0;
extern _DIS_PARAM dis_parameter;
#pragma vector = 25     //设置定时器4重载的中断向量号 = 25
__interrupt void TIM4_UPDATE_IRQHandler(void)
{
  TIM4_SR_bit.UIF = 0;  //清除中断标志
  ms_count++;           //中断使 ms_count++ 做加法，记录1毫秒产生的中断次数
  is_1ms = 1;
  if((ms_count %2)== 0)     //2毫秒点亮一位数码管
  {
    is_5ms = 1; 
    HC164D_Write_DU(Data[dis_parameter.dis_data[dis_parameter.active_bit]]);    //把 显示缓存的数值往段码芯片里写数据    
    HC164D_Write_WU( Wu[dis_parameter.active_bit]);             //写对应的位码   
    dis_parameter.active_bit++;   
    if(dis_parameter.active_bit >= BIT_NUMBER)    
    {     
      dis_parameter.active_bit = 0u ;  
    } 
  }
  if((ms_count %10) == 0)
  {
    is_10ms = 1;
  }
  
  if((ms_count %100) == 0)
  {
    is_100ms = 1;
  }
  
}

