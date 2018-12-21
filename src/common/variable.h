#pragma once

//编译相关宏
#define INIT_DSPLY          1            //main中是否显示初始化提示语
#define STATE_VAL_DISPLY    1            //main中是否显示电感读数和当前状态
#define SCOPE_SEND          1            //main中是否发送虚拟示波器值
#define ADC_VAL_DSPLY       0            //isr1中是否显示电感数值
#define LINEAR_TEST         0            //FSM_Ctrl中是否进入提线线性化测试


//adc采集参数
#define AD_BUFFER 20                     //惯性滤波器长度
#define AD_AVETIME 10                    //均值采样次数
#define AD_VALUE_1 1000                  //归一化精度
#define AD_ALPHA 0.8                     //惯性滤波权重

//adc通道选择
#define ADC0 ADC_CHANNEL_AD4             //PTB0
#define ADC1 ADC_CHANNEL_AD5             //PTB1
#define ADC2 ADC_CHANNEL_AD12            //PTF4
#define ADC3 ADC_CHANNEL_AD8             //PTC0
#define ADC4 ADC_CHANNEL_AD13            //PTF5
#define ADC5 ADC_CHANNEL_AD14            //PTF6

//以下为状态机状态范围
#define FSM_INIT 0                       //初始化状态
#define FSM_STRAIGHT 1                   //之道
#define FSM_CROSSROAD 2                  //十字路口
#define FSM_LEFT_CORNER 3                //左弯道
#define FSM_RIGHT_CORNER 4               //右弯道
#define FSM_S_TURN 5                     //连续弯
#define FSM_RAMP 6                       //坡道，废弃
#define FSM_RAMPTOP 7                    //坡道顶，废弃
#define FSM_GOAL 8                       //终点
#define FSM_OUT_OF_COURSE 255            //出界，占位


extern uint8_t current_State;           //状态机当前状态
extern uint16_t adc_res[6][AD_BUFFER];  //内部变量，请勿在adc.c以外修改数组值
extern uint8_t  adc_res_pos;            //内部变量，请勿在adc.c以外修改数组值
extern uint16_t adc_fine[6];            //归一化和惯性滤波后的值，提线使用，请勿在adc.c以外修改数组值
extern uint8_t flag_received;           //uart接受指示位
extern unsigned short OutData[4];     //虚拟示波器输出数值

//OLED显示内容
extern  unsigned char string0[];
extern  unsigned char string0_1[];
extern  unsigned char string1[];
extern  unsigned char string2_0[20];
extern  unsigned char string2_1[20];
extern  unsigned char string2_2[20];
extern  unsigned char string2_3[20];
extern  unsigned char string2_4[20];
extern  unsigned char string2_5[20];
extern  unsigned char string2_6[];
extern  unsigned char string2_7[];
extern  unsigned char string2_8[];
extern  unsigned char string2_9[];
extern  unsigned char string2_10[];
extern  unsigned char string2_11[];
extern  unsigned char string2_12[];

//数据格式转换函数
void itoa(int i, char *string);         //有符号int转字符串