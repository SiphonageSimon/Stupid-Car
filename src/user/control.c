#include "control.h"
float kp0 = 0.2;
float kp1 = 0.37;
float kp2 = 0.45;
float kp3 = 0.48;
int16_t last_error = 0;
int16_t error = 0; //偏差值
uint16_t newDuty; //最终输出占空比
int Spd_Offset = 0;

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
  //fuzzy_Ctrl(); //此函数仅调节Kp
  if(leftVal == 0 && rightVal == 0)
  {
     FTM_PWM_Duty(CFTM2, FTM_CH3, 0);//PWM0 PTB3              //�����
     FTM_PWM_Duty(CFTM2, FTM_CH4, 0);//PWM0 PTB2              //�����
     return;
  }
  if(midVal <= 1000)
  {
    error = 1000 - midVal;
    if(error < 150)
    {
      error *= kp0;
      Spd_Offset = 0;
    }
    else if(error < 200)
    {
      error *= kp1;
      Spd_Offset = 0;
     }
    else if(error < 500)
    {
      error *= kp2;
      Spd_Offset = 500;
    }
    else
    {
      error *= kp3;
      Spd_Offset = 800;
    }
    if(LRjudge > 0) //左减右为正，线位于车右侧，右转
    {
      error = error;
      FTM_PWM_Duty(CFTM2, FTM_CH3, 7000);//PWM0 PTB3              //�����
      FTM_PWM_Duty(CFTM2, FTM_CH4, 7000 - Spd_Offset);//PWM0 PTB2              //�����
    }
    else //左减右为负，线位于车左侧，左转
    {
      error = -error;
      FTM_PWM_Duty(CFTM2, FTM_CH3, 7000 - Spd_Offset);//PWM0 PTB3              //�����
      FTM_PWM_Duty(CFTM2, FTM_CH4, 7000);//PWM0 PTB2              //�����
    }
  }
  else
  {
    error = LRjudge * kp0;
    FTM_PWM_Duty(CFTM2, FTM_CH3, 8000);//PWM0 PTB3              //�����
    FTM_PWM_Duty(CFTM2, FTM_CH4, 8000);//PWM0 PTB2              //�����
  }
  if(error > TURN_MAX)
    error = TURN_MAX;
  else if(error < - TURN_MAX)
    error = -TURN_MAX;
  newDuty = newDuty + error ;
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