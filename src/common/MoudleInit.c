#include "include.h"

/*���뿪�س�ʼ��*/
void key_init(void)
{
    gpio_init(PTE6,0 ,0);       //���ڹ�һ��
    gpio_init(PTI5,0 ,0);
    gpio_init(PTI4,0 ,0);
    gpio_init(PTE4,0 ,0);
}

/*��д�����AD��ʼ��*/
void adc_init(void)                                               
{
    ADC_Init(ADC0, ADC_12BIT);//PTB10
    ADC_Init(ADC1, ADC_12BIT);//PTB10
    ADC_Init(ADC2, ADC_12BIT);//PTB10
    ADC_Init(ADC3, ADC_12BIT);//PTB10
    ADC_Init(ADC4, ADC_12BIT);//PTB10
    ADC_Init(ADC5, ADC_12BIT);//PTB10
}

/*�����ʼ��*/
void motor_init(void)
{
    gpio_init (PTE3,1,1);//������ʼ����ת
    gpio_init (PTI3,1,0);
    gpio_init (PTE1,1,1);//����ҳ�ʼ����ת
    gpio_init (PTG7,1,0);
    
    FTM_PWM_init(CFTM2, FTM_CH3,FTM_PTD1, 16700, 6700);//PWM0 PTB3              //�����
    FTM_PWM_init(CFTM2, FTM_CH4,FTM_PTG6, 16700, 6700);//PWM0 PTB2              //�����
}

void servo_init(void)
{
    FTM_PWM_init(CFTM1, FTM_CH1,FTM_PTC5, 100, SteerMid);
}