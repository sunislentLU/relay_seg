#ifndef _TRIGGER_H_
#define _TRIGGER_H_


#include<iostm8s105c6.h>
#define TRG_IN_3  PF_IDR_bit.IDR4 
//#define TRG_IN_4  PB_IDR_bit.IDR4
//#define TRG_IN_1  PA_IDR_bit.IDR1
//#define TRG_IN_2  PA_IDR_bit.IDR2


typedef enum
{
TRG_STATE_IDLE = 0x00,
TRG_STATE_CHG,
TRG_STATE_DOWN
}_TRG_STATUS;

typedef volatile union
{
unsigned char byte;
struct 
{
  unsigned char reserve :4;
  unsigned char trg_4_state:1;
  unsigned char trg_3_state:1;
  unsigned char trg_2_state:1;
  unsigned char trg_1_state:1;
}bit;
}_TRG_VALUE;

typedef enum
{
 TRG_INACTIVE = 0x00,
 TRG_ACTIVE
}_TRG_STATE;
void Trigger_Init();
void FollowTest();
unsigned char   Trg_Scan();
void ADC_Port_Init();
void Get_ADC_AnyChannal_Value(unsigned int *AD_Value);
#endif


