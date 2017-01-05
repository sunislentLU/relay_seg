#include<uart.h>


void Uart_Init(unsigned int baudrate)
{
  unsigned int baud;              //
  baud = 16000000 / baudrate;     //�趨������Ӧ�������봮��ʱ�ӵķ�Ƶ��
  //��дBRR2 , ��дBRR1
  UART2_BRR2 = ((unsigned char)((baud & 0xf000) >> 8 )) | ((unsigned char)(baud & 0x000f));       //��д���ر��ʵĸ�4λ���4λ
  UART2_BRR1 =  ((unsigned char)((baud & 0x0ff0) >> 4));                                          //��д���ر��ʵ��м�8λ
  
  UART2_CR1_bit.UART0 = 0;        //ʹ��UART3 
  UART2_CR2_bit.RIEN = 1;         //ʹ���жϽ���
  UART2_CR2_bit.TEN = 1;          //ʹ�ܷ���
  UART2_CR2_bit.REN = 1;          //����ʹ��
  
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
**�������ƣ�void UART2_Init(unsigned int baudrate)
**������������ʼ��USARTģ��
**��ڲ�����unsigned int baudrate  -> ���ô��ڲ�����
**�������
*******************************************************************************/
void UART2_Init(unsigned int baudrate)
{   
  unsigned int baud;              //
  
  baud = 16000000 / baudrate;     //�趨������Ӧ�������봮��ʱ�ӵķ�Ƶ��
  
  //��дBRR2 , ��дBRR1
  UART2_BRR2 = ((unsigned char)((baud & 0xf000) >> 8 )) | ((unsigned char)(baud & 0x000f));       //��д���ر��ʵĸ�4λ���4λ
  UART2_BRR1 =  ((unsigned char)((baud & 0x0ff0) >> 4));                                          //��д���ر��ʵ��м�8λ
  
  UART2_CR1_bit.UART0 = 0;        //ʹ��UART3 
  UART2_CR2_bit.RIEN = 1;         //ʹ���жϽ���
  UART2_CR2_bit.TEN = 1;          //ʹ�ܷ���
  UART2_CR2_bit.REN = 1;          //����ʹ��
  
}


/*******************************************************************************
**�������ƣ�void Uart2_SendData(unsigned char  data)
**�����������򴮿ڷ��ͼĴ���д��һ���ֽ�����
**��ڲ�����unsigned char  data
**�������
*******************************************************************************/
void Uart2_SendData(unsigned char  data)
{
  while(!(UART2_SR&0X80));        //�жϷ������ݼĴ����Ƿ�Ϊ��
  UART2_DR = data;                //���ͼĴ���д������
  while(!(UART2_SR&0X40));        //�ȴ��������    
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
/* FILE is typedef�� d in stdio.h. */ 


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
**��������:static void UART_putstr(const char *str) 
**��������:��USART1�����ַ���
**��ڲ���:*str �����ַ������׵�ַ
**���:�������ֱ�ӷ���
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
/*    ������    
int main(void)
{
number1 = 20;

asm("sim");                     //�ر�ϵͳ���ж�
CLK_CKDIVR = 0x00;              //CPUDIV = 1 HSIDIV = 1  �ڲ�ʱ�� = 16Mhz
UART2_Init(9600);               //���ô���2��ʼ�������������ò�����Ϊ9600 bps
asm("rim");                     //��ϵͳ���ж�
printf("\r\nTest!!");
while(1)                        //������ѭ�����ȴ����ڽ����ж�
{
delay(1000);
printf("\r\nNumber1 = %d , Number2 = %s" , number1 , number2);
  }
}*/




/*    ����2���������жϷ�����    */
#pragma vector = 23              //���ô���2�����ж������� = 23
__interrupt void UART2_RX_RXNE(void)
{          
  unsigned char ch1;
  
  UART2_SR_RXNE = 1;     //����жϱ�־
  ch1 = UART2_DR;        //�������ڽ��յ�������
  Uart2_SendData(ch1);   //�ѽ��յ���������ͨ�����ڷ��ͳ�ȥ
  
}