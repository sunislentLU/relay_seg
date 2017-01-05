#include<buzzer_led.h>

void Buzzer_Led_Init()
{
  PB_DDR_bit.DDR5 = 1;  //���ö˿�PD->7�������������Ĵ���Ϊ�������
  PB_CR1_bit.C15  = 1;  //����Ϊ�������
  PB_CR2_bit.C25  = 1;  //�����������Ϊ10MHZ
  LED1_OFF();
  PD_DDR_bit.DDR4 = 1;  //���ö˿�PD->7�������������Ĵ���Ϊ�������
  PD_CR1_bit.C14  = 1;  //����Ϊ�������
  PD_CR2_bit.C24  = 1;  //�����������Ϊ10MHZ
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