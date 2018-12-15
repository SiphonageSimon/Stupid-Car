#include "control.h"

float Kp = 0.4;
int16_t last_error = 0;
int16_t error = 0; //ƫ��ֵ
//���ƶ��
//����adc0Ϊ�У�adc1Ϊ��adc4Ϊ��
void servo_Ctrl(void)
{
  static int16_t leftVal, midVal, rightVal;
  uint8_t newDuty; //�������ռ�ձ�
  int16_t LRjudge; //�ж��������������ң������Ҹ�
  newDuty = SteerMid; //����
  leftVal = adc_fine[AD_LEFT];
  midVal = adc_fine[AD_MID];
  rightVal = adc_fine[AD_RIGHT];
  last_error = error; //�����ϴ�errorֵ
  LRjudge = leftVal - rightVal;
  
  if(midVal <= 1000)
  {
    error = 1000 - midVal;
      if(LRjudge > 0)  //�����Ϊ������λ�ڳ��Ҳ࣬��ת
    {
      error = error;
    }
    else              //�����Ϊ������λ�ڳ���࣬��ת
    {
      error = -error;
    }
    //fuzzy_Ctrl(); //�˺���������Kp
    error *= Kp;
    if(error > TURN_MAX)
      error = TURN_MAX;
  }
  else                //���µ�ʱֻʹ�������
  {
    error = LRjudge * Kp;
  }
  newDuty += error;
  FTM_PWM_Duty(CFTM1, FTM_CH1, newDuty);
}

//���������k���ѣ�����ʲô����ģ��
void fuzzy_Control(void)
{
  int16_t error_differ;
  error_differ = error - last_error;
  
}