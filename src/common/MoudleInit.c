#include "include.h"

/*拨码开关初始化*/
void key_init(void)
{
    gpio_init(PTE6,0 ,0);       //用于归一化
    gpio_init(PTI5,0 ,0);
    gpio_init(PTI4,0 ,0);
    gpio_init(PTE4,0 ,0);
}

/*电感传感器AD初始化*/
void adc_init(void)                                               
{
    ADC_Init(ADC0, ADC_12BIT);//PTB10
    ADC_Init(ADC1, ADC_12BIT);//PTB10
    ADC_Init(ADC2, ADC_12BIT);//PTB10
    ADC_Init(ADC3, ADC_12BIT);//PTB10
    ADC_Init(ADC4, ADC_12BIT);//PTB10
    ADC_Init(ADC5, ADC_12BIT);//PTB10
}

/*电机初始化*/
void motor_init(void)
{
    gpio_init (PTE3,1,1);//电机左初始化正转
    gpio_init (PTI3,1,0);
    gpio_init (PTE1,1,1);//电机右初始化正转
    gpio_init (PTG7,1,0);
    
    FTM_PWM_init(CFTM2, FTM_CH3,FTM_PTD1, 16700, 700);//PWM0 PTB3              //电机左
    FTM_PWM_init(CFTM2, FTM_CH4,FTM_PTG6, 16700, 700);//PWM0 PTB2              //电机右
}