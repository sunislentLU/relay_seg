#include<main.h>
#include<key.h>
#include<relay.h>
#include<segdis.h>
#include<uart.h>
#include<string.h>
#include<stdio.h>
#include<trigger.h>
#include<buzzer_led.h>

/*******************************variable and const***********************************/
const func relayOn_tab[RELAY_NUMBER]={Relay_1_On,Relay_2_On,Relay_3_On};
const func relayOff_tab[RELAY_NUMBER]={Relay_1_Off,Relay_2_Off,Relay_3_Off};
_DIS_PARAM dis_parameter;
_DELAY_VALUE  delay_tab[RELAY_NUMBER]= {{SET_DONE,RELAY_OFF,CHANGE_TYPE_NONE,{0,0,0,0,0,0,0},0},
{SET_DONE,RELAY_OFF,CHANGE_TYPE_NONE,{0,0,0,0,0,0,0},0},
{SET_DONE,RELAY_OFF,CHANGE_TYPE_NONE,{0,0,0,0,0,0,0},0}};

_RELAY_STATUS relay_state;
extern unsigned char  is_1ms;
extern unsigned char  is_10ms;
extern unsigned char  is_100ms;
extern unsigned char  is_5ms;

_RTC_VALUE rtc_value ={00,56,16,21,12,3,2016};
unsigned char test_process = 0;
_FIRST_MENU first_set_menu = FIRST_MENU_NONE;
_SET_NUMBER set_number =SET_NUMBER_1;
unsigned char menu_level = MUL_MENU_1;
_TIM_LIMIT time_limit[4]={{{9,0},{10,0}},{{16,0},{17,0}},{{21,0},{22,0}},{{4,0},{5,0}}};
_TIM_LIMIT limit_tmp[4]={{{0,0},{0,0}},{{0,0},{0,0}},{{0,0},{0,0}},{{0,0},{0,0}}};
_TIMING_VALUE timing_tmp;

_TRG_VALUE trg_state;
unsigned char trg_result = 0;
unsigned char dis_data_tmp[BIT_NUMBER] ={17,17,17,17};
static unsigned int dis_cnt = 0;
/******************* main function *****************************/
unsigned char is_entry_test = 0;
unsigned char setting = 0;
unsigned int adc_array[16]={0};
unsigned char adc_count = 0;
int main( void )
{
  unsigned int adc_temp = 0;
  unsigned char i = 0;
  asm("sim");
  CLK_CKDIVR = 0x00;  
  Hardware_Init();
  asm("rim");
  relay_state.relay_byte = 0;
  trg_state.byte = 0;
  // printf("all hardware gpios and interfaces init done and test the uart send datas\n\r");
#if (USED_RTC == 1)
  //Ds1302_Time_init(rtc_value.hour,rtc_value.min,rtc_value.second,rtc_value.year,rtc_value.month,rtc_value.day,rtc_value.week);
#endif
  memset(dis_parameter.dis_data,17,4);
  // DetectEntryTest();// ����1�����ⰴ�����½������ģʽ ���߰��°�������
  rtc_value = GetTime();
  CheckRelayTiming();
  while(1)
  {   
    if(is_1ms == 1)//1ms 
    {
      is_1ms = 0;
      if(is_entry_test == 1)
        FollowTest();
    }
    if(is_5ms == 1)// 5ms ��ʱʱ����Ӧ
    {
      is_5ms = 0;
      Get_ADC_AnyChannal_Value(&adc_temp);  
      adc_array[adc_count] = adc_temp;
      adc_count++;      
      if(adc_count >= 16)
      {
        adc_count = 0;
        for(i=0;i<16;i++)
        {
          adc_temp+=adc_array[i];
        }
        adc_temp>>=4;
        printf("the adc convert value is %d\n\r",adc_temp);
        
      }
    }
    if(is_10ms == 1)// 10ms ��ʱʱ����Ӧ
    {
      is_10ms = 0;
      Keyevent_Process();//�������Ͱ����������
      trg_result = SW_Scan();
      if(trg_result != 0xff)
      { 
        if(trg_result == 0x00)// û�д��� 
        {
          trg_state.byte =0x00;    
          CheckRelayTiming();//����ж���ʱ�������Ŀ���
        }
        if(trg_result&0x01)//����һ����
        {
          trg_state.bit.trg_1_state = TRG_ACTIVE;
        }
        else
          trg_state.bit.trg_1_state = TRG_INACTIVE;
        if(trg_result&0x02)
        {
          trg_state.bit.trg_2_state = TRG_ACTIVE;                                  
        }
        else
          trg_state.bit.trg_2_state = TRG_INACTIVE;
        if(trg_result&0x04)
        {
          trg_state.bit.trg_3_state = TRG_ACTIVE;        
        }
        else
          trg_state.bit.trg_3_state = TRG_INACTIVE;
        /***
        ��IO��û�б仯��ʱ�� SW_SCAN���� ���ص���0xff ��IO���б仯��ʱ��Ż�
        �ж��Ǵ�������û�д������ڴ���4 �����ʱ��ر����м̵���
        �ڴ���4 �Ǽ����ʱ���ж��Ƿ��ڴ�ʱ��Σ���������ٴδ�
        ���������ر�
        */        
        
        if(trg_result&0x08)// ����4 �����ж�
        {
          trg_state.bit.trg_4_state = TRG_ACTIVE;                
          if(relay_state.relay_byte != 0x00)
          {
            relayOff_tab[0]();
            relayOff_tab[1]();
            relayOff_tab[2]();
            relay_state.relay_byte = 0x00;
          }
        }
        else// û�д��� 
        {
          trg_state.bit.trg_4_state = TRG_INACTIVE;    
          CheckRelayTiming();//����ж���ʱ�������Ŀ���
        }
      }
    }
    else if(is_100ms == 1)// 100ms ��ʱʱ����Ӧ
    {
      is_100ms = 0;
      //CheckDelayOperation();// ����Ƿ�Ҫ�ӳٵĲ�����ʵ�ֲ���
#if (USED_RTC == 1)
      rtc_value = GetTime();// rtc ��ȡʱ��
#endif
      SegmentDisplay_Process();//�������ʾ����
      RetlayStateJude();
    }
  }
}

/*******************************************************/
/**function Name : TimeBase_Init**********************************/
/**function Describe: create a timebase at 1ms for loop use********/
void TimeBase_Init()
{
  TIM4_IER_bit.UIE = 0;           //�Ƚ�ֹ��ʱ��4�����ж�
  TIM4_EGR_bit.UG = 0;            //
  TIM4_PSCR_bit.PSC = 7;          //��ʱ��4ʱ�� = 16MHz / 128 = 125KHZ 
  
  TIM4_ARR = 125;                 //�趨1��ʱ������1������װ��ʱ�ļ���ֵ��255�����ֵ
  TIM4_CNTR = 0x00;               //�趨�������ĳ�ֵ
  
  TIM4_SR_bit.UIF = 0;            //��������жϱ�־
  TIM4_SR_bit.TIF = 0;            //��������жϱ�־
  TIM4_CR1_bit.CEN = 1;           //ʹ�ܶ�ʱ��4
  TIM4_CR1_bit.ARPE = 1;          //ʹ��Ԥװ�ؼĴ�������
  TIM4_IER_bit.UIE = 1;           //ʹ�ܸ����ж� 
}

/**function Name : Hardware_Init**********************************/
/** hole hardware initial********/
void Hardware_Init()
{
#if (USED_RTC == 1)
  Rtc_Init();
#endif
  Relay_Init();
  SegDis_Init();
  Uart_Init(9600);
  Key_Init();
  TimeBase_Init();
  Trigger_Init();
  Buzzer_Led_Init();
  ADC_Port_Init();
}


void Keyevent_Process()
{
  unsigned short key_value = 0;
  key_value = Key_scan();
  switch(key_value)
  {
    /*****************�̰�***************************/
    case(KEY_TYPE_PRESS<<8)|KEY_SUB:// ת����ȷ�ϰ���
      dis_cnt = 0;
      if(setting == 1)
      {
        printf("KEY_SUB press return \n\r");
        switch(first_set_menu)
        {
        case FIRST_MENU_MAIN:
          dis_data_tmp[0]--;
          if(dis_data_tmp[0]>2)
            dis_data_tmp[0] = 2;
          break;
        case FIRST_MENU_TIME:
        case FIRST_MENU_SET_TIMING:
          switch(set_number)
          {
          case SET_NUMBER_1:
            dis_data_tmp[3]--;
            if(dis_data_tmp[3]>9)
              dis_data_tmp[3]=9;
            break;
          case SET_NUMBER_2:
            dis_data_tmp[2]--;
            if(dis_data_tmp[2]>5)
              dis_data_tmp[2]=5;
            break;
          case SET_NUMBER_3:
            dis_data_tmp[1]--;
            if(dis_data_tmp[1]>9)
              dis_data_tmp[1]=9;
            break;
          case SET_NUMBER_4:
            dis_data_tmp[0]--;
            if(dis_data_tmp[0]>2)
              dis_data_tmp[0]=2;
            break;
          default:
            break;
          }
          break;
        default:
          break;
        }
      }
      else 
      {
        if(relay_state.relay_byte != 0x00)
        {
          relayOff_tab[0]();
          relayOff_tab[1]();
          relayOff_tab[2]();
          relay_state.relay_byte = 0x00;
        }
      }
      
      break;      
  case (KEY_TYPE_PRESS<<8)|KEY_ADD:// �Ӻż�
    dis_cnt = 0;
    printf("KEY_ADD press return \n\r");
    if(setting == 1)
    {
      switch(first_set_menu)
      {
      case FIRST_MENU_MAIN:
        dis_data_tmp[0]++;
        if(dis_data_tmp[0]>2)
          dis_data_tmp[0] = 0;
        break;
      case FIRST_MENU_TIME:
      case FIRST_MENU_SET_TIMING:
        switch(set_number)
        {
        case SET_NUMBER_1:
          dis_data_tmp[3]++;
          if(dis_data_tmp[3]>9)
            dis_data_tmp[3]=0;
          break;
        case SET_NUMBER_2:
          dis_data_tmp[2]++;
          if(dis_data_tmp[2]>5)
            dis_data_tmp[2]=0;
          break;
        case SET_NUMBER_3:
          dis_data_tmp[1]++;
          if(dis_data_tmp[1]>9)
            dis_data_tmp[1]=0;
          break;
        case SET_NUMBER_4:
          dis_data_tmp[0]++;
          if(dis_data_tmp[0]>2)
            dis_data_tmp[0]=0;
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
    }else
    {  
      if((relay_state.relay_byte == 0x00)&&(trg_state.bit.trg_4_state == 0))
      {        
        relayOn_tab[0]();
        relayOn_tab[1]();
        relayOn_tab[2]();
        relay_state.relay_byte = 0x07;
      }
    }
    break;   
  case (KEY_TYPE_PRESS<<8)|KEY_SET://���ð���
    dis_cnt = 0;
    if(setting == 1)
    {
      switch(first_set_menu)
      {
      case FIRST_MENU_NONE:
        first_set_menu = FIRST_MENU_MAIN;
        dis_parameter.state = DIS_STATE_HOLD;
        memset(dis_data_tmp,17,4);
        dis_data_tmp[0] = 0;
        break;
        //--------------------------------------------
      case FIRST_MENU_MAIN:
        if(dis_data_tmp[0] == 1)//����ʱ��
        {            
          dis_data_tmp[3] = rtc_value.min%10;    
          dis_data_tmp[2] = rtc_value.min/10;    
          dis_data_tmp[1] = rtc_value.hour%10;   
          dis_data_tmp[0] = rtc_value.hour/10;
          first_set_menu = FIRST_MENU_TIME;
          dis_parameter.state = DIS_STATE_SET_TIME; 
          set_number = SET_NUMBER_1;
        }
        else if(dis_parameter.dis_data[0] == 2)// ������ʱ
        {
          dis_data_tmp[1] = 1;
          set_number = SET_NUMBER_1;
          first_set_menu = FIRST_MENU_SET_TIMING;
          dis_parameter.state = DIS_STATE_NUMBER; 
        }
        else// û���˳�
        {
          first_set_menu = FIRST_MENU_NONE;
          dis_parameter.state = DIS_STATE_TIME;
        }
        
        break;
        //--------------------------------------------
      case FIRST_MENU_TIME://����ʱ��
        set_number++;// ����λ�� �� ʱ
        if(set_number>SET_NUMBER_4)
        {
          printf("time set over!\n\r");
          rtc_value.hour = dis_data_tmp[0]*10+dis_data_tmp[1];
          rtc_value.min = dis_data_tmp[2]*10+dis_data_tmp[3];
          DS1302SetHourMin(rtc_value.hour,rtc_value.min);
          dis_parameter.state = DIS_STATE_TIME;
          set_number = SET_NUMBER_1;
          first_set_menu = FIRST_MENU_NONE;
        }
        break;
        //----------------------------------------------------
      case FIRST_MENU_SET_TIMING:// ���ö�ʱ
        printf("the menu leve is %d\n\r",menu_level);
        switch(menu_level)
        {
        case MUL_MENU_1:  //���ùرտ�ʼʱ�� 
        case MUL_MENU_3:  
        case MUL_MENU_5:
        case MUL_MENU_7:
          set_number++;      
          if(set_number>SET_NUMBER_4)
          {  
            set_number = SET_NUMBER_1;
            timing_tmp.hour = dis_data_tmp[0]*10+dis_data_tmp[1];  
            timing_tmp.min = dis_data_tmp[2]*10+dis_data_tmp[3];
            printf("the hour is %d,the min is %d\n\r",timing_tmp.hour,timing_tmp.min);
            limit_tmp[menu_level/2].off_start_time = timing_tmp;
            menu_level++;
            memset(dis_data_tmp,0,4);
          }
          break;
        case MUL_MENU_2:// ���ùر�ֹͣʱ��
        case MUL_MENU_4:
        case MUL_MENU_6: 
        case MUL_MENU_8:
          set_number++;      
          if(set_number>SET_NUMBER_4)
          {  
            set_number = SET_NUMBER_1;
            timing_tmp.hour = dis_data_tmp[0]*10+dis_data_tmp[1];  
            timing_tmp.min = dis_data_tmp[2]*10+dis_data_tmp[3];  
            printf("the hour is %d,the min is %d\n\r",timing_tmp.hour,timing_tmp.min);
            limit_tmp[menu_level/2].off_stop_time = timing_tmp;
            menu_level++;
            memset(dis_data_tmp,17,4);
            dis_parameter.state = DIS_STATE_NUMBER;
            dis_data_tmp[2] = menu_level/2;
            if(menu_level>=8)// 6��ʱ���������
            {
              printf("timing set ok!\n\r");
              printf("stop time 1 is %d:%d to %d:%d /n/r",limit_tmp[0].off_start_time.hour \
                ,limit_tmp[0].off_start_time.min,limit_tmp[0].off_stop_time.hour \
                  ,limit_tmp[0].off_stop_time.min);
              printf("stop time 2 is %d:%d to %d:%d /n/r",limit_tmp[1].off_start_time.hour \
                ,limit_tmp[1].off_start_time.min,limit_tmp[1].off_stop_time.hour \
                  ,limit_tmp[1].off_stop_time.min );
              
              printf("stop time 3 is %d:%d to %d:%d /n/r",limit_tmp[2].off_start_time.hour \
                ,limit_tmp[2].off_start_time.min,limit_tmp[2].off_stop_time.hour \
                  ,limit_tmp[2].off_stop_time.min );           
              if(CheckTimingData()==0)
                printf("write timing time error\n\r");
              dis_parameter.state = DIS_STATE_TIME;      
              set_number = SET_NUMBER_1;      
              first_set_menu = FIRST_MENU_NONE;          
            }
          }
          break;
        default://0~7 ֮�� ����������ɺ�
          dis_parameter.state = DIS_STATE_TIME;           
          set_number = SET_NUMBER_1;                 
          first_set_menu = FIRST_MENU_NONE;
          
          break;
        }
        break;
        
      default:
        break;
      }
    }else
    {
      printf("......./n/r");
    }
    break;  
    /******************����**************************/
  case (KEY_TYPE_LONG_PRESS<<8)|KEY_SUB:// ��������һ���������ӳټ̵���1����
    printf("KEY_SUB long press  \n\r");
    
    break;
  case (KEY_TYPE_LONG_PRESS<<8)|KEY_ADD:
    printf("KEY_ADD long press  \n\r");
    
    break;
  case (KEY_TYPE_LONG_PRESS<<8)|KEY_SET:
    printf("KEY_SET long press  \n\r");
    
    break;   
    /*******************˫��*************************/ 
    
  case (KEY_TYPE_HOLD<<8)|KEY_SUB:
  case (KEY_TYPE_HOLD<<8)|KEY_ADD:
  case (KEY_TYPE_HOLD<<8)|KEY_SET:
    if(setting == 0)
    {
      printf("KEY_SET Hold press\n\r");
      setting = 1;
      LED1_ON();
    }
    else
    {
      dis_cnt = 0;
      if(first_set_menu == FIRST_MENU_TIME)
      {
        rtc_value.hour = dis_data_tmp[0]*10+dis_data_tmp[1];
        rtc_value.min = dis_data_tmp[2]*10+dis_data_tmp[3];
        DS1302SetHourMin(rtc_value.hour,rtc_value.min);
      }
      first_set_menu = FIRST_MENU_NONE;
      dis_parameter.state = DIS_STATE_TIME;
      set_number = SET_NUMBER_1; 
      setting = 0;
      LED1_OFF();
      
      
    }
    break;  
    
    
  default:
    break;
  }
  
}


void CheckRelayTiming()
{
  unsigned int limit_time_start[4];
  unsigned int limit_time_stop[4];
  unsigned int current_time;
  _TIMING_VALUE current_rtc;
  current_rtc.hour = rtc_value.hour;
  current_rtc.min = rtc_value.min;
  current_time = TimeChange2Number(current_rtc);
  limit_time_start[0] = TimeChange2Number(time_limit[0].off_start_time);
  limit_time_start[1] = TimeChange2Number(time_limit[1].off_start_time);
  limit_time_start[2] = TimeChange2Number(time_limit[2].off_start_time);
  limit_time_start[3] = TimeChange2Number(time_limit[3].off_start_time);
  limit_time_stop[0] = TimeChange2Number(time_limit[0].off_stop_time);
  limit_time_stop[1] = TimeChange2Number(time_limit[1].off_stop_time);
  limit_time_stop[2] = TimeChange2Number(time_limit[2].off_stop_time);
  limit_time_stop[3] = TimeChange2Number(time_limit[3].off_stop_time);
  if(((current_time>limit_time_start[0])&&(current_time < limit_time_stop[0]))||\
    ((current_time>limit_time_start[1])&&(current_time < limit_time_stop[1]))||\
      ((current_time>limit_time_start[2])&&(current_time < limit_time_stop[2]))||
        ((current_time>limit_time_start[3])&&(current_time < limit_time_stop[3]))  )
  {
    
    if(relay_state.relay_byte != 0x00)
    {
      relayOff_tab[0]();
      relayOff_tab[1]();
      relayOff_tab[2]();
      relay_state.relay_byte = 0x00;
    }
    
  }else
  {
    if(relay_state.relay_byte == 0x00)
    {
      relayOn_tab[0]();
      relayOn_tab[1]();
      relayOn_tab[2]();
      relay_state.relay_byte = 0x07;
    }
    
  }
}

#if (USED_RTC == 1)
/******************************************************************/
/**function name :  _RTC_VALUE GetTime()*/
/**function discribe get time and change to rtc value*/
/** return rtc value*/
/****************************************************************/
_RTC_VALUE GetTime()
{
  unsigned char bcd_tmp;
  _RTC_VALUE ret_rtcValue;
  get_time();
  bcd_tmp = table1[0]&0x7f;
  ret_rtcValue.second = BCD_Decimal(bcd_tmp);
  bcd_tmp = table1[1]&0x7f;
  ret_rtcValue.min = BCD_Decimal(bcd_tmp);
  if((table1[2]&0x80) == 0x00)//24 Сʱ
    bcd_tmp = table1[2]&0x3f;
  else
    bcd_tmp = table1[2]&0x1f;
  ret_rtcValue.hour = BCD_Decimal(bcd_tmp);
  bcd_tmp = table1[3]&0x3f;
  ret_rtcValue.day = BCD_Decimal(bcd_tmp);
  bcd_tmp = table1[4]&0x1f;
  ret_rtcValue.month = BCD_Decimal(bcd_tmp);
  bcd_tmp = table1[5]&0x07;
  ret_rtcValue.week = BCD_Decimal(bcd_tmp);
  bcd_tmp = table1[6]&0xff;
  ret_rtcValue.year = BCD_Decimal(bcd_tmp)+2000;
  return ret_rtcValue;
}
#endif 

/***************************************************/
/**function name SegmentDisplay_Process*/
/**function discribe segment led display control */
/***************************************************/
static unsigned char invert_cnt = 0;
void SegmentDisplay_Process()
{  
  
  if((first_set_menu != FIRST_MENU_NONE)||setting)
  {
    dis_cnt++;
    if(dis_parameter.state == DIS_STATE_NUMBER)
    {
      if((dis_cnt%3) == 0)
      {
        memset(dis_data_tmp,0,4);
        dis_parameter.state = DIS_STATE_SET_TIME;
        set_number = SET_NUMBER_1;
      }
    }    
    if(dis_cnt == 100)
    {
      dis_cnt = 0;
      first_set_menu = FIRST_MENU_NONE;
      dis_parameter.state = DIS_STATE_TIME;
      set_number = SET_NUMBER_1; 
      setting = 0;
      LED1_OFF();
    }
  }
  
  
  switch(dis_parameter.state)
  {
#if (USED_RTC == 1)
  case DIS_STATE_TIME:// ������ʾʱ��
    dis_parameter.dis_data[3] = rtc_value.min%10;
    dis_parameter.dis_data[2] = rtc_value.min/10;
    dis_parameter.dis_data[1] = rtc_value.hour%10;
    dis_parameter.dis_data[0] = rtc_value.hour/10;
    break;
#endif
  case DIS_STATE_SET_TIME:
    invert_cnt ++;
    if(invert_cnt <=8)
    {
      memcpy(dis_parameter.dis_data,dis_data_tmp,4);
      dis_parameter.dis_data[BIT_NUMBER - set_number-1] = dis_data_tmp[BIT_NUMBER - set_number-1];
    }
    else 
    {
      memcpy(dis_parameter.dis_data,dis_data_tmp,4);
      dis_parameter.dis_data[BIT_NUMBER - set_number-1] = 17;  
    }
    if(invert_cnt ==10)
      invert_cnt = 0;
    break;
  case DIS_STATE_NUMBER:  
    memcpy(dis_parameter.dis_data,dis_data_tmp,4); 
    break;
  case DIS_STATE_DECOUNT:// ����ʱ
    break;
  case DIS_STATE_HOLD:
    memcpy(dis_parameter.dis_data,dis_data_tmp,4);
    break;
  case DIS_STATE_TEST:
    dis_parameter.dis_data[0] = 16;
    dis_parameter.dis_data[1] = 16;
    dis_parameter.dis_data[2] = 16;
    dis_parameter.dis_data[3] = 16;
    break;
  default:
    break;
  }
}

void DetectEntryTest()
{
  static unsigned char test_det_cnt = 0;
  while(1)
  {
    if(ReadKeyNumber())
    {
      is_entry_test = 1;
      test_det_cnt = 0;
      dis_parameter.state = DIS_STATE_TEST;
      break;
    }
    else if(is_10ms == 1)
    {
      is_10ms = 0;
      test_det_cnt++;
      if(test_det_cnt>=10)
      {
        test_det_cnt = 0;
        break;
      }
      
    }
  }
}

unsigned char  CheckTimingData()
{
  unsigned char ret=0;
  unsigned char i=0;
  unsigned char correct_cnt = 0;
  for(i=0;i<4;i++)
  {
    if(limit_tmp[i].off_start_time.hour<limit_tmp[i].off_stop_time.hour)
      correct_cnt++;
  }
  
  for(i=0;i<3;i++)
  {
    
    if(limit_tmp[i].off_stop_time.hour<limit_tmp[i+1].off_start_time.hour)
      correct_cnt++;
  }
  if(correct_cnt>=7)
  {
    
    memcpy(time_limit,limit_tmp,16);
    ret = 1;
  }else
  {
    ret = 0; 
  }
  return ret;
}


void RetlayStateJude()
{
  if(rtc_value.second == 0)
  {
    if(((rtc_value.hour == time_limit[0].off_start_time.hour)&&(rtc_value.min == time_limit[0].off_start_time.min))||\
      ((rtc_value.hour == time_limit[1].off_start_time.hour)&&(rtc_value.min == time_limit[1].off_start_time.min))||\
        ((rtc_value.hour == time_limit[2].off_start_time.hour)&&(rtc_value.min == time_limit[2].off_start_time.min))||
          ((rtc_value.hour == time_limit[3].off_start_time.hour)&&(rtc_value.min == time_limit[3].off_start_time.min)))
    {
      if(relay_state.relay_byte != 0x00)
      {
        relayOff_tab[0]();
        relayOff_tab[1]();
        relayOff_tab[2]();
        relay_state.relay_byte = 0x00;
      }
      
    }
    else if(((rtc_value.hour == time_limit[0].off_stop_time.hour)&&(rtc_value.min == time_limit[0].off_stop_time.min))||\
      ((rtc_value.hour == time_limit[1].off_stop_time.hour)&&(rtc_value.min == time_limit[1].off_stop_time.min))||\
        ((rtc_value.hour == time_limit[2].off_stop_time.hour)&&(rtc_value.min == time_limit[2].off_stop_time.min))||
          ((rtc_value.hour == time_limit[3].off_stop_time.hour)&&(rtc_value.min == time_limit[3].off_stop_time.min)))
    {
      if(trg_state.bit.trg_4_state == 0)
      {
        if(relay_state.relay_byte == 0)
        {
          relayOn_tab[0]();
          relayOn_tab[1]();
          relayOn_tab[2]();
          relay_state.relay_byte = 0x07;
        }
      }
      
    }
  }
}

unsigned int TimeChange2Number(_TIMING_VALUE rtc)
{
  unsigned int ret = 0;
  ret = rtc.hour*60+rtc.min; 
  return ret;
}
