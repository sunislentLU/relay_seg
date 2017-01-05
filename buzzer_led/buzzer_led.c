#include<buzzer_led.h>

void Buzzer_Led_Init()
{
  PB_DDR_bit.DDR5 = 1;  //设置端口PD->7的输入输出方向寄存器为输出方向
  PB_CR1_bit.C15  = 1;  //设置为推挽输出
  PB_CR2_bit.C25  = 1;  //设置输出速率为10MHZ
  LED1_OFF();
  PD_DDR_bit.DDR4 = 1;  //设置端口PD->7的输入输出方向寄存器为输出方向
  PD_CR1_bit.C14  = 1;  //设置为推挽输出
  PD_CR2_bit.C24  = 1;  //设置输出速率为10MHZ
  BUZZER_OFF();
}


unsigned char buzzer_cnt = 0;
_BUZZER_TYPE buzzer_type = BUZZER_OFF;
void Buzzer_Process()
{
  switch(buzzer_type)
  {
  case BUZZER_OFF:
    BUZZER_OFF();
    break;
  case BUZZER_ON_SHORT:
    BUZZER_ON();
   buzzer_cnt++; 
   if(buzzer_cnt == 5)
   {
     buzzer_cnt = 0;
     buzzer_type = BUZZER_OFF;
   }
  case BUZZER_ON_LONG:
   BUZZER_ON();
   buzzer_cnt++; 
   if(buzzer_cnt == 12)
   {
     buzzer_cnt = 0;
     buzzer_type = BUZZER_OFF;
   }
    break;
    
  default:
    break;
  }



}