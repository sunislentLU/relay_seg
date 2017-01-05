#include<segdis.h>
#include<main.h>

//�����0 ~ F�Ĵ�������
unsigned char const Data[18] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0x00,0xff};
unsigned char const Wu[4] = {0x01,0x02,0x03,0x04};
/*******************************************************************************
**�������ƣ�void HC164D_Write_DU(unsigned char data)
**���������������оƬHC164Dд��������
**��ڲ�����unsigned char data
**�������
*******************************************************************************/

void SegDis_Init()
{
  //HC164оƬ λ��ʱ�����ų�ʼ��
  PB_DDR_bit.DDR0 = 1;  //���ö˿�PB->0�������������Ĵ���Ϊ�������
  PB_CR1_bit.C10  = 1;  //����Ϊ�������
  PB_CR2_bit.C20  = 1;  //�����������Ϊ10MHZ
  
  PD_DDR_bit.DDR7 = 1;  //���ö˿�PD->7�������������Ĵ���Ϊ�������
  PD_CR1_bit.C17  = 1;  //����Ϊ�������
  PD_CR2_bit.C27  = 1;  //�����������Ϊ10MHZ
  
  
  //HC164оƬ λ���������ų�ʼ��
  PC_DDR_bit.DDR6 = 1;  //���ö˿�PA->4�������������Ĵ���Ϊ�������
  PC_CR1_bit.C16  = 1;  //����Ϊ�������
  PC_CR2_bit.C26  = 1;  //�����������Ϊ10MHZ
  //HC164оƬ ����ʱ�����ų�ʼ��
  PC_DDR_bit.DDR7 = 1;  //���ö˿�PA->5�������������Ĵ���Ϊ�������
  PC_CR1_bit.C17  = 1;  //����Ϊ�������
  PC_CR2_bit.C27  = 1;  //�����������Ϊ10MHZ
  //HC164оƬ �����������ų�ʼ��
  PC_DDR_bit.DDR4 = 1;  //���ö˿�PA->6�������������Ĵ���Ϊ�������
  PC_CR1_bit.C14  = 1;  //����Ϊ�������
  PC_CR2_bit.C24  = 1;  //�����������Ϊ10MHZ
  
  PC_DDR_bit.DDR5 = 1;  //���ö˿�PA->6�������������Ĵ���Ϊ�������
  PC_CR1_bit.C15  = 1;  //����Ϊ�������
  PC_CR2_bit.C25  = 1;  //�����������Ϊ10MHZ
  
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
**�������ƣ�void HC164D_Write_WU(unsigned char data)
**������������λ��оƬHC164Dдλ������
**��ڲ�����unsigned char data
**�������
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
**�������ƣ�void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**���������������ʱ
**��ڲ�����unsigned int ms   ��������ʱ��ֵ
**�������
*******************************************************************************/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  ͨ��һ������ѭ��������ʱ*/
    for(y = 100 ; y > 0 ; y--);
}


unsigned char is_1ms = 0;
unsigned char is_10ms = 0;
unsigned char is_100ms = 0;
unsigned char is_5ms = 0;
unsigned char ms_count=0;
extern _DIS_PARAM dis_parameter;
#pragma vector = 25     //���ö�ʱ��4���ص��ж������� = 25
__interrupt void TIM4_UPDATE_IRQHandler(void)
{
  TIM4_SR_bit.UIF = 0;  //����жϱ�־
  ms_count++;           //�ж�ʹ ms_count++ ���ӷ�����¼1����������жϴ���
  is_1ms = 1;
  if((ms_count %2)== 0)     //2�������һλ�����
  {
    is_5ms = 1; 
    HC164D_Write_DU(Data[dis_parameter.dis_data[dis_parameter.active_bit]]);    //�� ��ʾ�������ֵ������оƬ��д����    
    HC164D_Write_WU( Wu[dis_parameter.active_bit]);             //д��Ӧ��λ��   
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

