#include<rtc.h>

//��������õ�ʱ�䣺   �롢  �֡�  ʱ�� �ա�  �¡�  ���ڡ� �ꡢ ������
const char table[] = {0x00, 0x28, 0x12, 0x09, 0x01, 0x05, 0x61, 0x80};

//�����ȡʱ������ڴ�ű��
unsigned char table1[7];

unsigned char read_byte();
void write_byte(unsigned char time_tx);

void Rtc_Init()
{
  PB_DDR_bit.DDR3 = 1;      //����DS1302��ʱ����Ϊ���
  PB_DDR_bit.DDR2 = 1;      //����DS1302��������������Ϊ���
  PD_DDR_bit.DDR0 = 1;      //����DS1302�ĸ�λ�ź�������Ϊ���
  
  PB_CR1_bit.C13 = 1;       //����PA2Ϊ�������
  // PB_CR1_bit.C12 = 1;       //����PA1Ϊ�������
  PD_CR1_bit.C12 = 1;       //����PG0Ϊ�������
  
  PB_CR2_bit.C23 = 1;       //����PB2Ϊ���������Ϊ10M
  // PB_CR2_bit.C22 = 1;       //����PB3Ϊ���������Ϊ10M
  PD_CR2_bit.C22 = 1;       //����PB4Ϊ���������Ϊ10M
  
  CLK_RESET();
  RST_RESET();
  RST_SET(); 
}

/****************************************************************************
* ��    �ƣ�set_time() 
* ��    �ܣ�����ʱ��
* ��ڲ�����
* ���ڲ�����
* ˵    ������ʾʱ����table[]�����У�ʱ���ʽһ����Ϊ����
bit7����bit4 bit3����bit0
ʮ��			��
****************************************************************************/
void set_time()
{
  int i;                         //����ѭ������
  
  RST_SET();                     //ʹ��DS1302
  write_byte(0xbe);            //ʱ�Ӷ��ֽ�д����
  for (i = 0;i < 8;i++)          //����д8���ֽ�����
  {
    write_byte(table[i]);     //����дһ���ֽں���
  }
  RST_RESET();                       //��λ
}
/****************************************************************************
* ��    �ƣ�set_time1() 
* ��    �ܣ�����ʱ��
* ��ڲ�����
* ���ڲ�����
* ˵    ������ʾʱ����table[]�����У�ʱ���ʽһ����Ϊ����
bit7����bit4 bit3����bit0
ʮ��			��
****************************************************************************/
void set_time1()
{
  int i;                         //����ѭ������
  
  RST_SET();                     //ʹ��DS1302
  write_byte(0xbe);            //ʱ�Ӷ��ֽ�д����
  for (i = 0;i < 7;i++)          //����д8���ֽ�����
  {
    write_byte(table1[i]);     //����дһ���ֽں���
  }
  write_byte(0x00);
  RST_RESET();                   //��λ
}
/****************************************************************************
* ��    �ƣ�get_time() 
* ��    �ܣ���ȡʱ��
* ��ڲ�����
* ���ڲ�����
* ˵    �����������ȫ��������table1[]
****************************************************************************/
void get_time()
{
  int i;                             //����ѭ������
  
  RST_SET();                         //ʹ��DS1302
  write_byte(0xbf);                //���Ͷ��ֽڶ�ȡ����
  for (i = 0 ; i < 7 ; i++)          //������ȡ7���ֽ�����
  {
    table1[i] = read_byte();      //���ö�ȡ1���ֽ����ݵĺ���
  }
  RST_RESET();                       //��λDS1302
}


extern void delay(unsigned int ms);
/****************************************************************************
* ��    �ƣ�time_write_1() 
* ��    �ܣ�дһ���ֽ�����
* ��ڲ�����unsigned char time_tx ��д����
* ���ڲ�����
* ˵    ����
****************************************************************************/
void write_byte(unsigned char dat)
{
  int j;                            //����ѭ������
  
  RST_SET();                       //ʹ��DS1302
  for (j = 0 ; j < 8; j++)         //����д8bit
  {
    
    CLK_RESET();          //ʱ���ź�����
    // delay(1);
    if(dat & 0x01)        //�жϴ����͵�����λ��0��1
    {
      DIO_SET();           //����������λ��1
    }
    else
    {
      DIO_RESET();         //����������Ϊ0
    }
    dat = dat >> 1;        //�����͵���������1λ
    CLK_SET();                     //����ʱ���ź�
  }
  //CLK_RESET();                    //д��һ���ֽڣ�����ʱ���ź�
  
}

/****************************************************************************
* ��    �ƣ�unsigned char read_byte() 
* ��    �ܣ���һ���ֽ�
* ��ڲ�����
* ���ڲ�����
* ˵    ����
****************************************************************************/
unsigned char read_byte()
{
  int j;                            //����ѭ������
  unsigned char  time_data;
  
  RST_SET();                       //ʹ��DS1302
  DIO_Set_InPut();                  //�������ݿڷ���Ϊ����
  for (j = 0 ;j < 8; j++)           //������ȡ8bit
  {
    CLK_SET();                       //����ʱ���ź�
    time_data = time_data >> 1;      //���ռĴ�������1λ
    CLK_RESET();                     //����ʱ���ź�
    if(DIO_Read())
    {
      time_data|= 0x80;
    }
  }
  DIO_Set_OutPut();                 //�ָ����ݿڷ���Ϊ���
  //CLK_RESET();                    //����ʱ���ź�
  
  return (time_data);                 //���ض�ȡ��������
}



void write_1302(unsigned char add , unsigned char dat)//��1302оƬд������ָ��д���ַ������
{
  RST_RESET();                      //ʧ��DS1302
  CLK_RESET();                     //����ʱ���ź�
  RST_SET();                       //ʹ��DS1302
  write_byte(add);
  write_byte(dat);
  CLK_SET();                       //����ʱ���ź�
  RST_RESET();                       //ʧ��DS1302
}

unsigned char read_1302(unsigned char add)//��1302�����ݺ�����ָ����ȡ������Դ��ַ
{
  unsigned char data;
  
  RST_RESET();                      //ʧ��DS1302
  CLK_RESET();                     //����ʱ���ź�
  RST_SET();                       //ʹ��DS1302
  write_byte(add);
  data = read_byte();
  CLK_SET();                       //����ʱ���ź�
  RST_RESET();                       //ʧ��DS1302
  
  return data;
}

unsigned char BCD_Decimal(unsigned char bcd)//BCD��תʮ���ƺ���������BCD������ʮ����
{
  unsigned char Decimal;
  
  Decimal = bcd >> 4;
  
  return (Decimal = Decimal * 10 + (bcd &= 0x0F));
}

unsigned char HEX_BCD(unsigned char hex)//ʮ���ƺ��� to BCD������ʮ����
{
  unsigned char Decimal;
  
  Decimal = hex / 10;
  Decimal <<= 4;
  Decimal |= (hex % 10);
  
  return Decimal;
}

void Ds1302_Time_init(unsigned char hour , unsigned char min , unsigned char second , unsigned int year_1 ,unsigned char month ,unsigned char day ,unsigned char week)//1302оƬ��ʼ���Ӻ���(2012-06-18,12:00:00,week7)?
{
  unsigned char middlenumble,year;
  RST_RESET();  //ʧ��DS1302
  CLK_RESET();  //����ʱ���ź�
  write_1302(0x8e,0x00);//����д����ֹд����
  
  write_1302(0x80, HEX_BCD(second));//��DS1302��д��Ĵ���80Hд���ʼ������
  
  write_1302(0x82,HEX_BCD(min));//��DS1302��д�ּĴ���82Hд���ʼ������
  
  write_1302(0x84,HEX_BCD(hour));//��DS1302��дСʱ�Ĵ���84Hд���ʼСʱ����
  
  write_1302(0x8a,week);//��DS1302��д�ܼĴ���8aHд���ʼ������
  
  write_1302(0x86,HEX_BCD(day));//��DS1302��д���ڼĴ���86Hд���ʼ��������
  
  write_1302(0x88,HEX_BCD(month));//��DS1302��д�·ݼĴ���88Hд���ʼ�·�����
  
  year = (unsigned char)(year_1 % 100);
  middlenumble = year / 10;
  middlenumble <<= 4;
  middlenumble |= (year % 10);
  write_1302(0x8c,middlenumble);//��DS1302��д��ݼĴ���8cHд���ʼ�������
  write_1302(0x8e,0x80);//��д����
}

void DS1302SetHourMin(unsigned char hour,unsigned char min)//1302оƬ��ʼ���Ӻ���(2012-06-18,12:00:00,week7)?
{
  unsigned char middlenumble;
  RST_RESET();  //ʧ��DS1302
  CLK_RESET();  //����ʱ���ź�
  write_1302(0x8e,0x00);//����д����ֹд����
  middlenumble = hour/10;
  middlenumble<<=4;
  middlenumble|=hour%10; 
  write_1302(0x84,middlenumble);//��DS1302��дСʱ�Ĵ���84Hд���ʼСʱ����
  middlenumble = min / 10;
  middlenumble <<= 4;
  middlenumble |= (min % 10);
  write_1302(0x82,middlenumble);//��DS1302��д�ּĴ���82Hд���ʼ������
  write_1302(0x8e,0x80);//��д����
}
