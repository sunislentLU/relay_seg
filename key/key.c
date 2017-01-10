#include<key.h>
/*******************************************************************************
**函数名称：void KEY1Init()
**功能描述：配置输入按键
**入口参数：无
**输出：无
*******************************************************************************/
void Key_Init()
{
  PC_DDR_bit.DDR1 = 0;      //GPC->PIN1 设置为输入模式
  PC_CR1_bit.C11 = 1;       //GPC->PIN1 带上拉电阻输入
  PC_CR2_bit.C21 = 0;       //GPC->PIN1  禁止外部中断
  
  PD_DDR_bit.DDR2 = 0;      //GPD->PIN2 设置为输入模式
  PD_CR1_bit.C12 = 1;       //GPD->PIN2 带上拉电阻输入
  PD_CR2_bit.C22 = 0;       //GPD->PIN2  禁止外部中断
  
  PD_DDR_bit.DDR3 = 0;      //GPD->PIN3 设置为输入模式
  PD_CR1_bit.C13 = 1;       //GPD->PIN3 带上拉电阻输入
  PD_CR2_bit.C23 = 0;       //GPD->PIN3  禁止外部中断
  SW_Init();
}

unsigned char  io_temp = 0;// temp to store last io state 
unsigned char  key_is_down =0;// flag use to judge double key press
static _KEY_STATE key_state = KEY_STATE_IDLE;// key state 
static unsigned char  key_counter = 0;	// time counter for key type judge and key debounce 
static unsigned short key_type = KEY_TYPE_NONE; // key type to return 
static unsigned char  key_number = 0;

unsigned char ReadKeyNumber()
{
  unsigned char key_number=0;
  if(KEY_1 == 0)
    key_number = 1;
  else if(KEY_2 == 0)
    key_number = 2;
  else if(KEY_3 == 0)
    key_number = 3;
  return key_number;
}


unsigned short Key_scan(void)
{  
  unsigned short  key_ret;   
  unsigned char io_tmp = 0;	
  io_tmp = ReadKeyNumber();
  switch(key_state)
  {
  case KEY_STATE_IDLE:
    if(io_tmp != io_temp)// state is changed
    {	
      // printf("io_tmp is %d/n/rio_temp is %d\n\r",io_tmp,io_temp);
      io_temp = io_tmp;              
      key_number  = io_temp;		
      key_state = KEY_STATE_DEBOUNCE;		
      key_counter = 0;		
      //printf("KEY_STATE_DEBOUNCE\n\r");
      break;
    }else if(key_type != KEY_TYPE_NONE)// have press or long press 
    {		
      key_counter++;		
      if(key_counter == 10)// 300ms no next press means to signal press		
      {			           
        key_ret = (key_type<<8)|key_number;			           
        key_type = KEY_TYPE_NONE;                             
        key_number = 0;	
        io_temp = io_tmp; 
        return key_ret;// 			          
      }		    
    }	     
    break;
  case KEY_STATE_DEBOUNCE:
    // printf("KEY_STATE_DEBOUNCE\n\r");
    if(io_temp == io_tmp)//debounce count			
      key_counter++;		
    else// jetter and return to idle and read io state 	
    {		
      key_counter = 0;		
      key_state = KEY_STATE_IDLE;		
      io_temp = io_tmp; // initial io_temp	
    }	
    if(key_counter == 3) // debounce success to judge press or long press	
    {	 
      key_counter = 0;	 
      key_state = KEY_STATE_DOWN;
      //printf("KEY_STATE_DOWN\n\r");
      break;	
    }	
    break;	
  case KEY_STATE_DOWN:	
    //printf("KEY_STATE_DOWN\n\r");
    if(io_temp == io_tmp)		
      key_counter++;	
    else // press	
    {	 
      key_counter = 0;	
      io_temp = io_tmp; // 		
      if(key_type == KEY_TYPE_NONE)
      {
        //  printf("KEY_TYPE_PRESS\n\r");
        key_type = KEY_TYPE_PRESS;	
      }	
      else if(key_type == KEY_TYPE_PRESS)			
        key_type = KEY_TYPE_DOUBLE_PRESS;		
      else 			
        key_type = KEY_TYPE_NONE;	 
      key_state = KEY_STATE_IDLE;// in the idle state to return key type	
    }	
    if(key_counter == 100)// long key active 	
    {	 
      key_counter = 0;          
      key_state = KEY_STATE_LONG;  
      //printf("KEY_STATE_LONG\n\r");
      break;             
    }
    
    break;	
  case KEY_STATE_LONG:	
    
    if(io_temp == io_tmp)			
      key_counter++;		
    else		
    {		
      key_counter = 0;		 
      if(key_type == KEY_TYPE_NONE)	
      {
        // printf("KEY_TYPE_LONG_PRESS\n\r");
        key_type = KEY_TYPE_LONG_PRESS;		
      }
      else if(key_type == KEY_TYPE_LONG_PRESS)//last key type is long press			 
        key_type = KEY_TYPE_DOUBLE_LPRESS;		 
      else // for key is hold and release			 
        key_type = KEY_TYPE_NONE;		 
      key_state = KEY_STATE_IDLE;		 
      //io_temp = io_tmp;		
      break;		
    }		
    if(key_counter == 100)// 3 second means hold 		
    {		 
      key_type = KEY_TYPE_HOLD;		 
      key_counter = 0;	
      // printf("KEY_TYPE_HOLD\n\r");
      io_temp = 0;	
      key_state = KEY_STATE_IDLE;
      key_ret = (key_type<<8)|key_number;
      key_type = KEY_TYPE_NONE;
      key_number = 0;
      return key_ret;		
    }	
    break;	
  default:		
    break;
  }
  return KEY_TYPE_NONE;//usually return 0;
}


void SW_Init()
{
  
  PB_DDR_bit.DDR4 = 0;      //GPB->PIN4 设置为输入模式
  PB_CR1_bit.C14 = 1;       //GPB->PIN4 带上拉电阻输入
  PB_CR2_bit.C24 = 0;       //GPB->PIN4  禁止外部中断
  
  PE_DDR_bit.DDR5 = 0;      //GPE->PIN5 设置为输入模式
  PE_CR1_bit.C15 = 1;       //GPE->PIN5 带上拉电阻输入
  PE_CR2_bit.C25 = 0;       //GPE->PIN5  禁止外部中断
}

unsigned char Read_SwitchIO()
{
  unsigned char io_state = 0xff;

  if(SW_2 == 0)
    io_state = 2;
  else if(SW_1 == 0)
    io_state = 1;
  else if((SW_1 == 1)&&(SW_2 == 1))
    io_state = 0;
  return io_state;
}


_SW_STATE sw_status = SW_STATE_IDLE;
unsigned char sw_io_tmp =0xff;
unsigned char sw_cnt = 0;



unsigned char SW_Scan()
{
  unsigned char sw_io = 0xff;
  unsigned char ret =0xff;
  sw_io = Read_SwitchIO();
  switch(sw_status)
  {
  case SW_STATE_IDLE:
    if(sw_io != sw_io_tmp)
    {
      sw_io_tmp = sw_io;
      sw_cnt = 0;
      sw_status = SW_STATE_CHG;
    }
    break;
  case SW_STATE_CHG:
    if(sw_io != sw_io_tmp)
      sw_status = SW_STATE_IDLE;
    else
      sw_cnt++;
    if(sw_cnt == 100)//1000ms
      sw_status = SW_STATE_DWN;
    break; 
  case SW_STATE_DWN:
    if(sw_io_tmp == sw_io)
    {
      ret = sw_io_tmp;
      sw_status = SW_STATE_IDLE;
      return ret;
    }
    else
      ret = 0xff;
    sw_status = SW_STATE_IDLE;
    break; 
  }
  return 0xff;
}


