#include<trigger.h>
#include<main.h>


void Trigger_Init()
{
  
  PA_DDR_bit.DDR1 = 0;      //GPC->PIN1 设置为输入模式
  PA_CR1_bit.C11 = 1;       //GPC->PIN1 带上拉电阻输入
  PA_CR2_bit.C21 = 0;       //GPC->PIN1  禁止外部中断
 
  PA_DDR_bit.DDR2 = 0;      //GPD->PIN2 设置为输入模式
  PA_CR1_bit.C12 = 1;       //GPD->PIN2 带上拉电阻输入
  PA_CR2_bit.C22 = 0;       //GPD->PIN2  禁止外部中断
  
  PF_DDR_bit.DDR4 = 0;      //GPD->PIN3 设置为输入模式
  PF_CR1_bit.C14 = 1;       //GPD->PIN3 带上拉电阻输入
  PF_CR2_bit.C24 = 0;       //GPD->PIN3  禁止外部中断
  
//  PB_DDR_bit.DDR4 = 0;      //GPD->PIN3 设置为输入模式
//  PB_CR1_bit.C14 = 1;       //GPD->PIN3 带上拉电阻输入
//  PB_CR2_bit.C24 = 0;       //GPD->PIN3  禁止外部中断
  
  
}


void FollowTest()
{
  
  PC_ODR_bit.ODR2 = PA_IDR_bit.IDR1;//relay 1
  PC_ODR_bit.ODR3 = PA_IDR_bit.IDR2; //relay 2
  PE_ODR_bit.ODR5 = PF_IDR_bit.IDR4; //relay 3
  PB_ODR_bit.ODR5 = PB_IDR_bit.IDR4;
  
}

unsigned char  Read_TriggerIO()
{
  unsigned char ret = 0;
//  if(TRG_IN_4 == 1)
//    ret |=1;
  ret<<=1;
  if(TRG_IN_3 == 1) 
    ret |=1;
  ret<<=1;
//  if(TRG_IN_2 == 1) 
//    ret |=1;
  ret<<=1;
//  if(TRG_IN_1 == 1) 
//    ret |=1;
  return ret;
}

_TRG_STATUS  trg_status = TRG_STATE_IDLE;

unsigned char trg_io_tmp;
static unsigned char trg_cnt = 0;


unsigned char  Trg_Scan()
{
  unsigned char trg_io;
  unsigned char ret =0xff;
  trg_io = Read_TriggerIO();
  switch(trg_status)
  {
  case TRG_STATE_IDLE:
    if(trg_io != trg_io_tmp)
    {
      trg_io_tmp = trg_io;
      trg_cnt = 0;
      trg_status = TRG_STATE_CHG;
    }
    break;
  case TRG_STATE_CHG:
    if(trg_io != trg_io_tmp)
      trg_status = TRG_STATE_IDLE;
    else
      trg_cnt++;
    if(trg_cnt == 100)//1000ms
      trg_status = TRG_STATE_DOWN;
    break; 
  case TRG_STATE_DOWN:
    if(trg_io_tmp == trg_io)
    {
      ret = trg_io_tmp;
    }
    else
      ret = 0xff;
    trg_status = TRG_STATE_IDLE;
    break; 
  }
  return ret;
}


#if(TRG_USE_ADC == 1)

/*******************************************************************************
**函数名称：void ADC_Init()
**功能描述：初始化ADC
**入口参数：无
**输出：无
*******************************************************************************/
void ADC_Port_Init()
{
  PB_DDR_bit.DDR1 = 0;   //设置PB->6 为输入
  PB_CR1_bit.C11 = 0;    //设置为悬空输入 
  PB_CR2_bit.C21 = 0;    //设置中断禁止
  
  ADC_CR1_bit.SPSEL = 0;    //fmaster / 18 = 16MHZ / 18 = 888888HZ
  ADC_CR2_bit.ALIGN = 1;    //RIGHT ALIGN
  ADC_TDRH = 0x00;          // 禁止触发功能 
  ADC_TDRL = 0x02;
  ADC_CSR_bit.CH = 1;       //SELECT AIN6
  ADC_CR1_bit.CONT = 0; 
  ADC_CR1_bit.ADON = 1;     //启动ADC
}


/*******************************************************************************
**函数名称：void Get_ADC_AnyChannal_Value(unsigned int *AD_Value , unsigned int SamplesTimes)
**功能描述：读取ADC完成多次模数转换结果
**入口参数：unsigned int *AD_Value  存放多冷采样的平均值
            unsigned int SamplesTimes  采样次数
**输出：无
*******************************************************************************/
void Get_ADC_AnyChannal_Value(unsigned int *AD_Value)
{
  //unsigned int SamplesAdd , count , i;
  //SamplesAdd = 0;
  unsigned int count;
  //for(i = 0 ; i < SamplesTimes ; i++ )
  //{
    ADC_CR1_bit.ADON = 1;     //启动ADC
    while(ADC_CSR_bit.EOC == 0);  //等待转换结束
    count = ADC_DRH;          //先读取高8位
    count = (unsigned int)((count << 8) + ADC_DRL);   //高8位与低8位相加，凑成16位数据
  //  SamplesAdd += count;
    ADC_CSR_bit.EOC = 0;
  //}
  *AD_Value = count;
}
#endif
