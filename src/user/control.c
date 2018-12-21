#include "control.h"

float kp[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
int16_t last_error = 0; //上一次偏差值
int16_t error = 0;      //提线偏差值
int16_t newDuty;       //最终输出占空比
int Spd_Offset = 0;     //差速
int16_t leftVal, midVal, rightVal,leftVerVal,rightVerVal;
Threshold threshold;
void FSM_select(void)//有限状态机跳转
{
  switch (current_State)
  {
  case FSM_INIT:
    threshold.enter_crossroad = 600; //标定各个状态切换阈值
    threshold.enter_corner = 200;
    threshold.enter_straight = 100;
    threshold.no_val = 0;
    current_State = FSM_STRAIGHT;
    break;
  case FSM_STRAIGHT:
    if(leftVerVal > threshold.enter_crossroad && rightVerVal > threshold.enter_crossroad)
      current_State = FSM_STRAIGHT; //FSM_CROSSROAD;
    else if(leftVerVal > threshold.enter_corner && rightVerVal < threshold.enter_corner)
      current_State = FSM_LEFT_CORNER;
    else if(leftVerVal < threshold.enter_corner && rightVerVal > threshold.enter_corner)
      current_State = FSM_RIGHT_CORNER;
    break;
  case FSM_LEFT_CORNER:
    if((leftVerVal < threshold.enter_straight && rightVerVal < threshold.enter_straight))
      current_State = FSM_STRAIGHT;
    else if(leftVerVal > threshold.enter_crossroad && rightVerVal > threshold.enter_crossroad)
      current_State = FSM_STRAIGHT; //FSM_CROSSROAD;
    break;
  case FSM_RIGHT_CORNER:
    if((leftVerVal < threshold.enter_straight && rightVerVal < threshold.enter_straight))
      current_State = FSM_STRAIGHT;
    else if(leftVerVal > threshold.enter_crossroad && rightVerVal > threshold.enter_crossroad)
      current_State = FSM_STRAIGHT; //FSM_CROSSROAD;
    break;
  case FSM_S_TURN:
    break;
  case FSM_CROSSROAD:
    if(leftVerVal > threshold.enter_corner && rightVerVal < threshold.enter_corner)
      current_State = FSM_LEFT_CORNER;
    else if(leftVerVal < threshold.enter_corner && rightVerVal > threshold.enter_corner)
      current_State = FSM_RIGHT_CORNER;
    break;
  case FSM_OUT_OF_COURSE:
    break;
  default:
    break;    
  }
}

void FSM_Ctrl(void)
{
  int16_t LRjudge; //判断线在做还是在右，左正右负
  const float linear_ratio1 = 3.5; //用于线性化的参数
  const float compensate_ratio0 = 700.0;
  const float compensate_ratio1 = 0.667;
  const float compensate_ratio2 = 450.0; //左电感修正系数
  const float compensate_ratio3 = 650.0; //左电感修正系数
  newDuty = SteerMid; //回中
  
  leftVal = adc_fine[AD_LEFT];
  midVal = adc_fine[AD_MID];
  rightVal = adc_fine[AD_RIGHT];
  leftVerVal = adc_fine[AD_LEFT_VERTICAL];
  rightVerVal = adc_fine[AD_RIGHT_VERTICAL];
  
  last_error = error; //储存上次error值
  LRjudge = leftVal - rightVal;
  
  //提线线性化部分
  if(midVal > 1000)
  {
    error = LRjudge;
  }
  else if(leftVal > midVal && leftVal > rightVal)
  {
    error = linear_ratio1 * (1000 - midVal) - compensate_ratio1 * (1000 - midVal - compensate_ratio0) - compensate_ratio2;
  }
  else if(rightVal > midVal && rightVal > leftVal)
  {
    error = linear_ratio1 * (1000 - midVal) - compensate_ratio1 * (1000 - midVal - compensate_ratio0) - compensate_ratio3;
    error = -error;
  }
  else
  {
    error = LRjudge;
  }
#if LINEAR_TEST
  if(error < 0)
    error = -error;
  if(LRjudge <0)
    LRjudge = -LRjudge;
  GetData(error,LRjudge,0,0,OutData);
#endif

  FSM_select();//状态跳转

  switch(current_State)
  {
  case FSM_STRAIGHT:
    newDuty += error * kp[0];
    break;
  case FSM_LEFT_CORNER:
    if(error > 0)
      newDuty += error * kp[1];
    break;
  case FSM_RIGHT_CORNER:
    if(error < 0)
      newDuty += error * kp[1];
    break;
  case FSM_S_TURN:
    break;
  default:
    break;
  }
  
  if(newDuty > SteerMid + TURN_MAX)
    newDuty = SteerMid + TURN_MAX;
  else if(newDuty < SteerMid - TURN_MAX)
    newDuty = SteerMid - TURN_MAX;
  FTM_PWM_Duty(CFTM1, FTM_CH1, newDuty);
  
  motor_Ctrl(6800,6800);
  return;
}

//速度闭环函数，目前用作单纯速度给定
void motor_Ctrl(uint16_t left_Spd, uint16_t right_Spd)
{
  FTM_PWM_Duty(CFTM2, FTM_CH3, left_Spd);   //PWM0 PTB3
  FTM_PWM_Duty(CFTM2, FTM_CH4, right_Spd);  //PWM0 PTB2   
  return;
}

void servo_Ctrl(void)//烂得一匹
{
  int16_t LRjudge; //判断线在做还是在右，左正右负
  newDuty = SteerMid; //回中
  leftVal = adc_fine[AD_LEFT];
  midVal = adc_fine[AD_MID];
  rightVal = adc_fine[AD_RIGHT];
  leftVerVal = adc_fine[AD_LEFT_VERTICAL];
  rightVerVal = adc_fine[AD_RIGHT_VERTICAL];
  last_error = error; //储存上次error值
  LRjudge = leftVal - rightVal;
  FSM_select();//状态跳转
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
      //error *= kp0;
      Spd_Offset = 0;
    }
    else if(error < 200)
    {
      //error *= kp1;
      Spd_Offset = 0;
     }
    else if(error < 500)
    {
      //error *= kp2;
      Spd_Offset = 100;
    }
    else
    {
      //error *= kp3;
      Spd_Offset = 200;
    }
    switch(current_State)
    {
    case FSM_STRAIGHT:
      error *= kp[0];
      break;
    default:
      error *= kp[3];
      break;
    }
    if(LRjudge > 0) //左减右为正，线位于车右侧，右转
    {
      error = error;
      FTM_PWM_Duty(CFTM2, FTM_CH3, 6800);//PWM0 PTB3              //�����
      FTM_PWM_Duty(CFTM2, FTM_CH4, 6800 - Spd_Offset);//PWM0 PTB2              //�����
    }
    else //左减右为负，线位于车左侧，左转
    {
      error = -error;
      FTM_PWM_Duty(CFTM2, FTM_CH3, 6800 - Spd_Offset);//PWM0 PTB3              //�����
      FTM_PWM_Duty(CFTM2, FTM_CH4, 6800);//PWM0 PTB2              //�����
    }
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

uint8_t getCurrentState(void)
{
  return current_State;
}