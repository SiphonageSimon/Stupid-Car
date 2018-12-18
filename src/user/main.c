/*****************************************************************
【dev.env.】IAR7.80.4
【Target  】S9KEAZ128
【Crystal 】16.000Mhz
【busclock】40.000MHz
【pllclock】40.000MHz
******************************************************************/
/*************************测试说明******************************/
/****************************************************************
  测试什么功能就去掉对应函数前的“//”
  该例程针对mini核心板以及配套母版编写，其他版本核心板修改引脚
  TF卡&OLED版本核心板上OLED接口与ADC例程中的ADC引脚冲突，注意修改
  PWM和编码器采集函数不能同时启用，因为FTM模块冲突
  每个测试函数都写有 while（1），同时使能两个测试函数先执行的函数有效
*******************************************************************/
/******************************************************************
接口定义：
-------------------------------------------------------------
LED         单片机接口
//核心板上RGB灯珠
LED0           PTC2
LED1           PTB4
LED2           PTE5
//母板上LED灯珠
LED3           PTI4
LED4           PTH6
===============================================================
KEY        单片机接口
//核心板上按键
KEY0          PTA0
//母板按键
KEY1          PTB5
KEY2          PTH2
KEY3          PTE6
===============================================================
拨码开关    单片机接口
KEY0          PTI2
KEY1          PTI3
KEY2          PTE2
KEY3          PTE3
===============================================================
TSL1401模块     单片机接口
VCC             5V
GND             GND
SI              I5/D5
SCK             I6/D6
ADC             AD8 C0/AD9 C1
===============================================================
多路电感模块    单片机接口
VCC             5V
GND             GND
ADC通道         管脚关系     
ADC12           F4      
ADC13           F5      
ADC14           F6      
ADC15           F7       
ADC4            B0      
ADC5            B1   
ADC6            B2       
ADC7            B3
-------------------------------------------------------------
电源监控或者它用    
ADC11           C3              
-------------------------------------------------------------
MPU6050        单片机接口   FLEXCOMM8
VCC             5V
GND             GND
SDA1            H3
SCL1            H4
//////////////////////////////////////////////////////////通用部分功能//////////
电机驱动        单片机接口   
VCC             5V
PWM1            FTM2-CH0  H0
PWM2            FTM2-CH1  H1
PWM3            FTM2-CH2  D0
PWM4            FTM2-CH3  D1
-------------------------------------------------------------
舵机接口        单片机接口
VCC             可调
GND             GND
PWM1            FTM1-CH1  E7
-------------------------------------------------------------
龙邱512编码器   单片机接口   
VCC             5V
GND             GND      
LSB/A           FTM0 E0    
DIR/B           H7     
LSB/A           FTM1 E7     
DIR/B           H5     
-------------------------------------------------------------
OLED模块        单片机接口
VCC             5V
GND             GND
SCK             F3
SDA             F2
RST             A7
DC              A6
-------------------------------------------------------------
蓝牙/USBTTL    单片机接口   FLEXCOMM0
VCC             5V
GND             GND
UART2_RX        I1    
UART2_TX        I0
=============================================================*/
//编译运行


#include "include.h"

void main(void)
{
  DisableInterrupts ;
  
  key_init();
  adc_init();
  motor_init();
  servo_init();
  OLED_Init();//OLED initialization
  OLED_BufferClear();//set black screen after OLED initialization
  OLED_BufferFlashAll();//flash screen, clear power-off frags
#if INIT_DSPLY //init hints
  OLED_DrawString8X16(0,0,string0,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  OLED_DrawString8X16(0,13,string0_1,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  OLED_BufferFlash();
#endif
  FTM_PWM_Duty(CFTM1, FTM_CH1, SteerMid);
  AD_value_init(); 
  //PIT_Init(PIT_CHANNEL0,2); //init isr0
  PIT_Init(PIT_CHANNEL1,2); //init isr1
  //RTC_Init(RTC_LPOCLK, 1);
  
  EnableInterrupts;
  
  while(1)
  { 
    //OLED_BufferFill();
    //OLED_BufferFlash();
    //time_delay_ms(30);
    //OLED_BufferClear();
    //OLED_BufferFlash();
    //time_delay_ms(30);
    //servo_Ctrl();
    OLED_BufferClear();
    itoa(adc_fine[0], string2_0);
    itoa(adc_fine[1], string2_1);
    itoa(getErrorVal(), string2_2);
    //itoa(adc_fine[2], string2_2);
    itoa(adc_fine[3], string2_3);
    itoa(adc_fine[4], string2_4);
    //itoa(adc_fine[5], string2_5);
    itoa(getNewDutyVal(), string2_5);
    OLED_DrawString8X16(30,42,string2_0,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30,21,string2_1,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30,0,string2_2,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(0,42,string2_6,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(0,21,string2_7,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    //OLED_DrawString8X16(0,0,string2_8,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(OLED_DRAW_WIDTH_MAX/2,42,string2_9,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    //OLED_DrawString8X16(OLED_DRAW_WIDTH_MAX/2,21,string2_10,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    //OLED_DrawString8X16(OLED_DRAW_WIDTH_MAX/2,0,string2_11,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30+OLED_DRAW_WIDTH_MAX/2,42,string2_3,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    //OLED_DrawString8X16(30+OLED_DRAW_WIDTH_MAX/2,21,string2_4,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30+OLED_DRAW_WIDTH_MAX/2,0,string2_5,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_BufferFlash();
  } 
}
