#include<uart.h>


void Uart_Init(unsigned int baudrate)
{
  unsigned int baud;              //
  baud = 16000000 / baudrate;     //设定串口相应波特率与串口时钟的分频数
  //先写BRR2 , 再写BRR1
  UART2_BRR2 = ((unsigned char)((baud & 0xf000) >> 8 )) | ((unsigned char)(baud & 0x000f));       //先写波特比率的高4位与低4位
  UART2_BRR1 =  ((unsigned char)((baud & 0x0ff0) >> 4));                                          //后写波特比率的中间8位
  
  UART2_CR1_bit.UART0 = 0;        //使能UART3 
  UART2_CR2_bit.RIEN = 1;         //使能中断接收
  UART2_CR2_bit.TEN = 1;          //使能发送
  UART2_CR2_bit.REN = 1;          //接收使能
  
}


#ifdef UART_USE_STDIO
#define UART_printf(fmt,args...)    printf (fmt ,##args)
#endif



#ifdef UART_USE_STDIO
int printf(const char *fmt, ...);
#endif

unsigned int number1 ;
const char number2[] = "Test Printf OK!!";




/*******************************************************************************
**函数名称：void UART2_Init(unsigned int baudrate)
**功能描述：初始化USART模块
**入口参数：unsigned int baudrate  -> 设置串口波特率
**输出：无
*******************************************************************************/
void UART2_Init(unsigned int baudrate)
{   
  unsigned int baud;              //
  
  baud = 16000000 / baudrate;     //设定串口相应波特率与串口时钟的分频数
  
  //先写BRR2 , 再写BRR1
  UART2_BRR2 = ((unsigned char)((baud & 0xf000) >> 8 )) | ((unsigned char)(baud & 0x000f));       //先写波特比率的高4位与低4位
  UART2_BRR1 =  ((unsigned char)((baud & 0x0ff0) >> 4));                                          //后写波特比率的中间8位
  
  UART2_CR1_bit.UART0 = 0;        //使能UART3 
  UART2_CR2_bit.RIEN = 1;         //使能中断接收
  UART2_CR2_bit.TEN = 1;          //使能发送
  UART2_CR2_bit.REN = 1;          //接收使能
  
}


/*******************************************************************************
**函数名称：void Uart2_SendData(unsigned char  data)
**功能描述：向串口发送寄存器写入一个字节数据
**入口参数：unsigned char  data
**输出：无
*******************************************************************************/
void Uart2_SendData(unsigned char  data)
{
  while(!(UART2_SR&0X80));        //判断发送数据寄存器是否为空
  UART2_DR = data;                //向发送寄存器写入数据
  while(!(UART2_SR&0X40));        //等待发送完成    
}

int fputc(int ch, FILE *f)
{
  Uart2_SendData(ch);
  return ch;
}

#ifdef __CC_ARM // MDK Support
struct __FILE 
{ 
  int handle;
  /* Whatever you require here. If the only file you are using is */ 
  /* standard output using printf() for debugging, no file handling */ 
  /* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 


#elif __ICCARM__ /* IAR support */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  size_t nChars = 0;
  if (buffer == 0)
  {
    /* This means that we should flush internal buffers.  Since we*/
    /* don't we just return.  (Remember, "handle" == -1 means that all*/
    /* handles should be flushed.)*/
    return 0;
  }
  /* This function only writes to "standard out" and "standard err",*/
  /* for all other file handles it returns failure.*/
  if ((handle != _LLIO_STDOUT) && (handle != _LLIO_STDERR))
  {
    return _LLIO_ERROR;
  }
  /* Send data.*/
  while (size--)
  {
    UART_WriteByte(UART_DebugInstance, *buffer++);
    ++nChars;
  }
  return nChars;
}

size_t __read(int handle, unsigned char * buffer, size_t size)
{
  size_t nChars = 0;
  uint16_t ch = 0;
  if (buffer == 0)
  {
    /* This means that we should flush internal buffers.  Since we*/
    /* don't we just return.  (Remember, "handle" == -1 means that all*/
    /* handles should be flushed.)*/
    return 0;
  }
  /* This function only writes to "standard out" and "standard err",*/
  /* for all other file handles it returns failure.*/
  if ((handle != _LLIO_STDIN) && (handle != _LLIO_STDERR))
  {
    return _LLIO_ERROR;
  }
  /* read data.*/
  while (size--)
  {
    while(UART_ReadByte(UART_DebugInstance, &ch));
    *buffer++ = (char)ch & 0xFF;
    ++nChars;
  }
  return nChars;
}


#endif /* comiler support */


/*******************************************************************************
**函数名称:static void UART_putstr(const char *str) 
**功能描述:向USART1发送字符串
**入口参数:*str 发送字符串的首地址
**输出:发送完毕直接返回
*******************************************************************************/
static void UART_putstr(const char *str)
{
  while(*str != '\0')
  {
    Uart2_SendData( *str++);
  }
}

static void printn(unsigned int n, unsigned int b)
{
  static char *ntab = "0123456789ABCDEF";
  unsigned int a, m;
  if (n / b)
  {
    a = n / b;
    printn(a, b);  
  }
  m = n % b;
  Uart2_SendData( ntab[m]);
}

int UART_printf(const char *fmt, ...)
{
  char c;
  unsigned int *adx = (unsigned int*)(void*)&fmt + 1;
_loop:
  while((c = *fmt++) != '%')
  {
    if (c == '\0') return 0;
    Uart2_SendData(c);
  }
  c = *fmt++;
  if (c == 'd' || c == 'l')
  {
    printn(*adx, 10);
  }
  if (c == 'o' || c == 'x')
  {
    printn(*adx, c=='o'? 8:16 );
  }
  if (c == 's')
  {
    UART_putstr((char*)*adx);
  }
  adx++;
  goto _loop;
  //return 0;
} 
/*    主函数    
int main(void)
{
number1 = 20;

asm("sim");                     //关闭系统总中断
CLK_CKDIVR = 0x00;              //CPUDIV = 1 HSIDIV = 1  内部时钟 = 16Mhz
UART2_Init(9600);               //调用串口2初始化函数，并设置波特率为9600 bps
asm("rim");                     //打开系统总中断
printf("\r\nTest!!");
while(1)                        //进入死循环，等待串口接收中断
{
delay(1000);
printf("\r\nNumber1 = %d , Number2 = %s" , number1 , number2);
  }
}*/




/*    串口2接收数据中断服务函数    */
#pragma vector = 23              //设置串口2接收中断向量号 = 23
__interrupt void UART2_RX_RXNE(void)
{          
  unsigned char ch1;
  
  UART2_SR_RXNE = 1;     //清除中断标志
  ch1 = UART2_DR;        //读出串口接收到的数据
  Uart2_SendData(ch1);   //把接收到的数据再通过串口发送出去
  
}