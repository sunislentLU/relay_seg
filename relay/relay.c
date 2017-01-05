#include<relay.h>


void Relay_Init(void)
{
  PC_DDR_bit.DDR2 = 1;  //���ö˿�PB->0�������������Ĵ���Ϊ�������
  PC_CR1_bit.C12  = 1;  //����Ϊ�������
  PC_CR2_bit.C22  = 1;  //�����������Ϊ10MHZ
  
  PC_DDR_bit.DDR3 = 1;  //���ö˿�PD->7�������������Ĵ���Ϊ�������
  PC_CR1_bit.C13  = 1;  //����Ϊ�������
  PC_CR2_bit.C23  = 1;  //�����������Ϊ10MHZ
   
  PE_DDR_bit.DDR5 = 1;  //���ö˿�PD->7�������������Ĵ���Ϊ�������
  PE_CR1_bit.C15  = 1;  //����Ϊ�������
  PE_CR2_bit.C25  = 1;  //�����������Ϊ10MHZ
  
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


