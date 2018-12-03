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

//蜜汁无法编译于是补了这个东西
#define OLED_WIDTH                      128
#define OLED_HEIGHT                     64
#define OLED_TRUE                       0x01
#define OLED_FALSE                      0x00
#define OLED_FLASH_FLAG_BOUNDARY        ((OLED_WIDTH*OLED_HEIGHT/8)/2)
#define OLED_BYTE_NUMBER                ((OLED_WIDTH*OLED_HEIGHT/8)/8)
#define OLED_SCREEN_FLIPPING            OLED_FALSE

#define OLED_COLOR_WHITE                0x1
#define OLED_COLOR_BLACK                0x0
#define OLED_DRAW_WIDTH_MAX             (OLED_WIDTH-1)
#define OLED_DRAW_HEIGHT_MAX            (OLED_HEIGHT-1)
#define OLED_DRAW_PI                    3.1415
#define OLED_DRAW_SQRT(value)           sqrt(value)
#define OLED_ANGLE_0                    0
#define OLED_ANGLE_90                   1
#define OLED_ANGLE_180                  2
#define OLED_ANGLE_270                  3
///////////////////////////////////////////////

void main(void)
{

  DisableInterrupts ;                  //禁止中断 
  /*
  ADC_CHANNEL_AD0 -------------- A0   
  ADC_CHANNEL_AD1 -------------- A1
  ADC_CHANNEL_AD2 -------------- A6 
  ADC_CHANNEL_AD3 -------------- A7
  ADC_CHANNEL_AD4 -------------- B0
  ADC_CHANNEL_AD5 -------------- B1  
  ADC_CHANNEL_AD6 -------------- B2
  ADC_CHANNEL_AD7 -------------- B3
  ADC_CHANNEL_AD8 -------------- C0
  ADC_CHANNEL_AD9 -------------- C1 
  ADC_CHANNEL_AD10 ------------- C2
  ADC_CHANNEL_AD11 ------------- C3
  ADC_CHANNEL_AD12 ------------- F4
  ADC_CHANNEL_AD13 ------------- F5 
  ADC_CHANNEL_AD14 ------------- F6
  ADC_CHANNEL_AD15 ------------- F7
  */
  
  key_init();
  adc_init();
  motor_init();
  OLED_Init();
  //对OLED与库进行初始化
  OLED_BufferClear();
  //初始化后，对画面进行全黑操作，OLED_BufferClear()可进行全白操作
  OLED_BufferFlashAll();
  //初始化后，进行一次全屏刷新，防止意外断电导致显示屏残留画像
  OLED_DrawString8X16(0,0,string0,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  OLED_DrawString8X16(0,13,string0_1,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  OLED_BufferFlash();
  //OLED_Init();
  //OLED_BufferClear();
  //OLED_BufferFlashAll();
  //OLED_DrawString8X16(0,0,string0,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  //OLED_BufferFlash();
  AD_value_init();
  PIT_Init(PIT_CHANNEL0,5);
  PIT_Init(PIT_CHANNEL1,500);
  //RTC_Init(RTC_LPOCLK, 1);
  
  
  
  //对画面进行输出
  
  EnableInterrupts;     //开启总中断   
  
  while(1)
  { 
    
    //OLED_BufferFill();
    //OLED_BufferFlash();
    //time_delay_ms(30);
    //OLED_BufferClear();
   // OLED_BufferFlash();
    //time_delay_ms(30);
    
  } 
}
