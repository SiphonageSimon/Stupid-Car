#include "control.h"

float Kp = 0.2;
int16_t last_error = 0;
int16_t error = 0; //偏差值
uint16_t newDuty; //最终输出占空比
//控制舵机
//其中adc0为中，adc1为左，adc3为右
void servo_Ctrl(void)
{
  static int16_t leftVal, midVal, rightVal;

  int16_t LRjudge; //判断线在做还是在右，左正右负
  newDuty = SteerMid; //回中
  leftVal = adc_fine[AD_LEFT];
  midVal = adc_fine[AD_MID];
  rightVal = adc_fine[AD_RIGHT];
  last_error = error; //储存上次error值
  LRjudge = leftVal - rightVal;
  
  if(midVal <= 200)
  {
        //fuzzy_Ctrl(); //此函数仅调节Kp
    error = 1000 - midVal;
        error *= Kp;
    if(error > TURN_MAX)
      error = TURN_MAX;
      if(LRjudge > 0)  //左减右为正，线位于车右侧，右转
    {
      error = error;
    }
    else              //左减右为负，线位于车左侧，左转
    {
      error = -error;
    }

  }
  else                //上坡道时只使用左减右
  {
    error = LRjudge * 0.2;
  }
  newDuty += error;
  FTM_PWM_Duty(CFTM1, FTM_CH1, newDuty);
}

//调整舵机的k而已，不是什么正经模糊
void fuzzy_Control(void)
{
  int16_t error_differ;
  error_differ = error - last_error;
  
}

int getErrorVal(void)
{
  return error;
}

uint16_t getNewDutyVal(void)
{
  return newDuty; 
}