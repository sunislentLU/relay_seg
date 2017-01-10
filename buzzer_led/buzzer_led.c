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
  BEEP_CSR_bit.BEEPDIV = 0x02;         //8 * (5 + 2) = 56分频
  BEEP_CSR_bit.BEEPSEL = 0x00;         //蜂鸣器的输出频率 = Fls / ( 8 * (BEEPDIV + 2) )= 128K / (8 * (5 + 2)) = 2285HZ
  BEEP_CSR_bit.BEEPEN = 1;
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
    if(buzzer_cnt >= 5)
    {
      buzzer_cnt = 0;
      buzzer_type = BUZZER_OFF;
    }
  case BUZZER_ON_LONG:
    BUZZER_ON();
    buzzer_cnt++; 
    if(buzzer_cnt >= 10)
    {
      buzzer_cnt = 0;
      buzzer_type = BUZZER_OFF;
    }
    break;
    
  default:
    break;
  }
}

_LED_TYPE led_flash_type = LED_FLASH_TWO;

void led_Flash_Process()
{
  static unsigned char led_count = 0; 
  switch(led_flash_type) 
  {
  case LED_ALWAYS_ON:
    LED1_ON();
    break;
  case LED_FLASH_ONE:
    if(led_count == 0)
      LED1_ON();
    led_count++;
    if(led_count == 5)
      LED1_OFF();
    if(led_count == 10)
      led_count = 0 ;
    break;
  case LED_FLASH_TWO:
    
    if(led_count == 0)
      LED1_ON();
    led_count++;
    if(led_count == 2)
      LED1_OFF();
    if(led_count == 3)
      LED1_ON();
    if(led_count == 4)
      LED1_OFF();
    if(led_count == 10)
      led_count = 0;
   
    break;  
  case LED_FLASH_TRIPLE:
    if(led_count == 0)
      LED1_ON();
    led_count++;
    if(led_count == 2)
      LED1_OFF();
    if(led_count == 3)
      LED1_ON();
    if(led_count == 4)
      LED1_OFF();
    if(led_count == 5)
      LED1_ON();
    if(led_count == 6)
      LED1_OFF();
    if(led_count == 10)   
      led_count = 0;
    
    break;
  case LED_ALWAYS_OFF:
    LED1_OFF();
    break;
  case LED_TYPE_NONE:
    break;
  default:
    break;
  }
  
  
  
  
}