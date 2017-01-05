#ifndef _MAIN_H_
#define _MAIN_H_
#include <rtc.h>
#define RELAY_NUMBER  3
#define BIT_NUMBER  4
#define USED_RTC  1
#define KEY_SET  KEY_NUMBER_3
#define KEY_ADD  KEY_NUMBER_2
#define KEY_SUB  KEY_NUMBER_1


typedef enum
{
 CHANGE_TYPE_NONE = 0x00,
 CHANGE_TYPE_ON,
 CHANGE_TYPE_OFF
}_CHANGE_TYPE;
typedef enum
{
RELAY_OFF = 0x00,
RELAY_ON
}_RELAY_STATE;

typedef enum
{
SET_DONE = 0x00,
SETTING
}_SET_STATE;

typedef struct 
{
_SET_STATE settting;
_RELAY_STATE state;
_CHANGE_TYPE change_type;
_RTC_VALUE expired_time;
unsigned int delay_second;
}_DELAY_VALUE;

typedef volatile union
{
 unsigned char relay_byte;
 struct
 {
   unsigned char reserve:5;
   unsigned char relay_3_state:1;
   unsigned char relay_2_state:1;
   unsigned char relay_1_state:1;
 }relay_bit;
}_RELAY_STATUS;

typedef enum
{
  DIS_STATE_TIME = 0x00,// 显示时间
  DIS_STATE_NUMBER,
  DIS_STATE_SET_TIME,//设置个位
  DIS_STATE_DECOUNT,// 倒计时
  DIS_STATE_TEST,
  DIS_STATE_HOLD
}_DIS_STATE;// 显示状态

typedef struct 
{
 _DIS_STATE state;// 显示状态
 unsigned char dis_data[BIT_NUMBER];// 显示内容
 unsigned char active_bit;// 当前显示位
}_DIS_PARAM;
typedef void (*func)(void);
typedef enum
{
FIRST_MENU_NONE = 0x00,
FIRST_MENU_MAIN,
FIRST_MENU_TIME,
FIRST_MENU_SET_TIMING
}_FIRST_MENU;

typedef enum
{
MUL_MENU_1 =0x00,  
MUL_MENU_2,
MUL_MENU_3,
MUL_MENU_4,
MUL_MENU_5,
MUL_MENU_6,
MUL_MENU_7,
MUL_MENU_8
}_MUL_MENU;
typedef struct
{
  unsigned char hour;
  unsigned char min;
}_TIMING_VALUE;

typedef struct
{
 _TIMING_VALUE off_start_time;
 _TIMING_VALUE off_stop_time;
}_TIM_LIMIT;

typedef struct
{
  unsigned char is_1ms;
  unsigned char is_10ms;
  unsigned char is_100ms;
  unsigned char is_5ms;
  unsigned char ms_count;
}_MAINTIME;
typedef enum
{
SET_NUMBER_1=0x00,
SET_NUMBER_2,
SET_NUMBER_3,
SET_NUMBER_4
}_SET_NUMBER;

extern _MAINTIME maintime;

extern void Keyevent_Process();
extern  void CheckRelayTiming();
extern _RTC_VALUE GetTime();
extern void SegmentDisplay_Process();
extern void TimeBase_Init();
extern void Hardware_Init();
extern void DetectEntryTest();
unsigned char  CheckTimingData();
unsigned int TimeChange2Number(_TIMING_VALUE rtc);
void RetlayStateJude();

#endif