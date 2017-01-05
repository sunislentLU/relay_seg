#include<rtc.h>

//定义待设置的时间：   秒、  分、  时、 日、  月、  星期、 年、 控制字
const char table[] = {0x00, 0x28, 0x12, 0x09, 0x01, 0x05, 0x61, 0x80};

//定义读取时间和日期存放表格
unsigned char table1[7];

unsigned char read_byte();
void write_byte(unsigned char time_tx);

void Rtc_Init()
{
  PB_DDR_bit.DDR3 = 1;      //设置DS1302的时钟线为输出
  PB_DDR_bit.DDR2 = 1;      //设置DS1302的数据线先设置为输出
  PD_DDR_bit.DDR0 = 1;      //调置DS1302的复位信号线设置为输出
  
  PB_CR1_bit.C13 = 1;       //设置PA2为推挽输出
  // PB_CR1_bit.C12 = 1;       //设置PA1为推挽输出
  PD_CR1_bit.C12 = 1;       //设置PG0为推挽输出
  
  PB_CR2_bit.C23 = 1;       //设置PB2为的输出速率为10M
  // PB_CR2_bit.C22 = 1;       //设置PB3为的输出速率为10M
  PD_CR2_bit.C22 = 1;       //设置PB4为的输出速率为10M
  
  CLK_RESET();
  RST_RESET();
  RST_SET(); 
}

/****************************************************************************
* 名    称：set_time() 
* 功    能：设置时间
* 入口参数：
* 出口参数：
* 说    明：出示时间在table[]数组中，时间格式一日期为例：
bit7——bit4 bit3——bit0
十天			天
****************************************************************************/
void set_time()
{
  int i;                         //定义循环变量
  
  RST_SET();                     //使能DS1302
  write_byte(0xbe);            //时钟多字节写命令
  for (i = 0;i < 8;i++)          //连续写8个字节数据
  {
    write_byte(table[i]);     //调用写一个字节函数
  }
  RST_RESET();                       //复位
}
/****************************************************************************
* 名    称：set_time1() 
* 功    能：设置时间
* 入口参数：
* 出口参数：
* 说    明：出示时间在table[]数组中，时间格式一日期为例：
bit7——bit4 bit3——bit0
十天			天
****************************************************************************/
void set_time1()
{
  int i;                         //定义循环变量
  
  RST_SET();                     //使能DS1302
  write_byte(0xbe);            //时钟多字节写命令
  for (i = 0;i < 7;i++)          //连续写8个字节数据
  {
    write_byte(table1[i]);     //调用写一个字节函数
  }
  write_byte(0x00);
  RST_RESET();                   //复位
}
/****************************************************************************
* 名    称：get_time() 
* 功    能：读取时间
* 入口参数：
* 出口参数：
* 说    明：结果放在全局数组中table1[]
****************************************************************************/
void get_time()
{
  int i;                             //设置循环变量
  
  RST_SET();                         //使能DS1302
  write_byte(0xbf);                //发送多字节读取命令
  for (i = 0 ; i < 7 ; i++)          //连续读取7个字节数据
  {
    table1[i] = read_byte();      //调用读取1个字节数据的函数
  }
  RST_RESET();                       //复位DS1302
}


extern void delay(unsigned int ms);
/****************************************************************************
* 名    称：time_write_1() 
* 功    能：写一个字节数据
* 入口参数：unsigned char time_tx 待写数据
* 出口参数：
* 说    明：
****************************************************************************/
void write_byte(unsigned char dat)
{
  int j;                            //设置循环变量
  
  RST_SET();                       //使能DS1302
  for (j = 0 ; j < 8; j++)         //连续写8bit
  {
    
    CLK_RESET();          //时钟信号拉低
    // delay(1);
    if(dat & 0x01)        //判断待发送的数据位是0或1
    {
      DIO_SET();           //待发送数据位是1
    }
    else
    {
      DIO_RESET();         //先设置数据为0
    }
    dat = dat >> 1;        //待发送的数据右移1位
    CLK_SET();                     //拉高时钟信号
  }
  //CLK_RESET();                    //写完一个字节，拉低时钟信号
  
}

/****************************************************************************
* 名    称：unsigned char read_byte() 
* 功    能：读一个字节
* 入口参数：
* 出口参数：
* 说    明：
****************************************************************************/
unsigned char read_byte()
{
  int j;                            //设置循环变量
  unsigned char  time_data;
  
  RST_SET();                       //使能DS1302
  DIO_Set_InPut();                  //设置数据口方向为输入
  for (j = 0 ;j < 8; j++)           //连续读取8bit
  {
    CLK_SET();                       //拉高时钟信号
    time_data = time_data >> 1;      //接收寄存器右移1位
    CLK_RESET();                     //拉低时钟信号
    if(DIO_Read())
    {
      time_data|= 0x80;
    }
  }
  DIO_Set_OutPut();                 //恢复数据口方向为输出
  //CLK_RESET();                    //拉低时钟信号
  
  return (time_data);                 //返回读取到的数据
}



void write_1302(unsigned char add , unsigned char dat)//向1302芯片写函数，指定写入地址，数据
{
  RST_RESET();                      //失能DS1302
  CLK_RESET();                     //拉低时钟信号
  RST_SET();                       //使能DS1302
  write_byte(add);
  write_byte(dat);
  CLK_SET();                       //拉高时钟信号
  RST_RESET();                       //失能DS1302
}

unsigned char read_1302(unsigned char add)//从1302读数据函数，指定读取数据来源地址
{
  unsigned char data;
  
  RST_RESET();                      //失能DS1302
  CLK_RESET();                     //拉低时钟信号
  RST_SET();                       //使能DS1302
  write_byte(add);
  data = read_byte();
  CLK_SET();                       //拉高时钟信号
  RST_RESET();                       //失能DS1302
  
  return data;
}

unsigned char BCD_Decimal(unsigned char bcd)//BCD码转十进制函数，输入BCD，返回十进制
{
  unsigned char Decimal;
  
  Decimal = bcd >> 4;
  
  return (Decimal = Decimal * 10 + (bcd &= 0x0F));
}

unsigned char HEX_BCD(unsigned char hex)//十进制函数 to BCD，返回十进制
{
  unsigned char Decimal;
  
  Decimal = hex / 10;
  Decimal <<= 4;
  Decimal |= (hex % 10);
  
  return Decimal;
}

void Ds1302_Time_init(unsigned char hour , unsigned char min , unsigned char second , unsigned int year_1 ,unsigned char month ,unsigned char day ,unsigned char week)//1302芯片初始化子函数(2012-06-18,12:00:00,week7)?
{
  unsigned char middlenumble,year;
  RST_RESET();  //失能DS1302
  CLK_RESET();  //拉低时钟信号
  write_1302(0x8e,0x00);//允许写，禁止写保护
  
  write_1302(0x80, HEX_BCD(second));//向DS1302内写秒寄存器80H写入初始秒数据
  
  write_1302(0x82,HEX_BCD(min));//向DS1302内写分寄存器82H写入初始分数据
  
  write_1302(0x84,HEX_BCD(hour));//向DS1302内写小时寄存器84H写入初始小时数据
  
  write_1302(0x8a,week);//向DS1302内写周寄存器8aH写入初始周数据
  
  write_1302(0x86,HEX_BCD(day));//向DS1302内写日期寄存器86H写入初始日期数据
  
  write_1302(0x88,HEX_BCD(month));//向DS1302内写月份寄存器88H写入初始月份数据
  
  year = (unsigned char)(year_1 % 100);
  middlenumble = year / 10;
  middlenumble <<= 4;
  middlenumble |= (year % 10);
  write_1302(0x8c,middlenumble);//向DS1302内写年份寄存器8cH写入初始年份数据
  write_1302(0x8e,0x80);//打开写保护
}

void DS1302SetHourMin(unsigned char hour,unsigned char min)//1302芯片初始化子函数(2012-06-18,12:00:00,week7)?
{
  unsigned char middlenumble;
  RST_RESET();  //失能DS1302
  CLK_RESET();  //拉低时钟信号
  write_1302(0x8e,0x00);//允许写，禁止写保护
  middlenumble = hour/10;
  middlenumble<<=4;
  middlenumble|=hour%10; 
  write_1302(0x84,middlenumble);//向DS1302内写小时寄存器84H写入初始小时数据
  middlenumble = min / 10;
  middlenumble <<= 4;
  middlenumble |= (min % 10);
  write_1302(0x82,middlenumble);//向DS1302内写分寄存器82H写入初始分数据
  write_1302(0x8e,0x80);//打开写保护
}
