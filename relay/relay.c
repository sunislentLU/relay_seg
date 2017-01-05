#include<relay.h>


void Relay_Init(void)
{
  PC_DDR_bit.DDR2 = 1;  //设置端口PB->0的输入输出方向寄存器为输出方向
  PC_CR1_bit.C12  = 1;  //设置为推挽输出
  PC_CR2_bit.C22  = 1;  //设置输出速率为10MHZ
  
  PC_DDR_bit.DDR3 = 1;  //设置端口PD->7的输入输出方向寄存器为输出方向
  PC_CR1_bit.C13  = 1;  //设置为推挽输出
  PC_CR2_bit.C23  = 1;  //设置输出速率为10MHZ
   
  PE_DDR_bit.DDR5 = 1;  //设置端口PD->7的输入输出方向寄存器为输出方向
  PE_CR1_bit.C15  = 1;  //设置为推挽输出
  PE_CR2_bit.C25  = 1;  //设置输出速率为10MHZ
  
  RELAY_1_OFF();
  RELAY_2_OFF();
  RELAY_3_OFF();
  
}

void Relay_1_On(void)
{
RELAY_1_ON();
}

void Relay_2_On(void)
{
RELAY_2_ON();
}

void Relay_3_On(void)
{
RELAY_3_ON();
}

void Relay_1_Off(void)
{
RELAY_1_OFF();
}

void Relay_2_Off(void)
{
RELAY_2_OFF();
}

void Relay_3_Off(void)
{
RELAY_3_OFF();
}


