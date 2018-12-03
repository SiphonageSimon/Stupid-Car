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
int UART4_FLAG=0;
int EventCount=0,ti=0,in=0;
//串口0接收中断服务例程
void UART0_ISR(void)
{
  uint8_t ReData;
  uint8_t txt[20];
  
  DisableInterrupts ;//关总中断
  
  ReData = Uart_GetChar(UARTR0);
  sprintf((char*)txt,"UART0_RX: %c \n",ReData);  
  Uart_SendString(UARTR0,txt);
  
  EnableInterrupts;   //开总中断
}


//串口1接收中断服务例程
void UART1_ISR(void)
{
  uint8_t ReData;
  uint8_t txt[20];
  
  DisableInterrupts ;//关总中断
  
  ReData = Uart_GetChar(UARTR1);
  sprintf((char*)txt,"UART1_RX: %c \n",ReData);  
  Uart_SendString(UARTR1,txt);
  
  EnableInterrupts;   //开总中断
}

//串口2接收中断服务例程
void UART2_ISR(void)
{
  uint8_t ReData;
  uint8_t txt[20];
  
  DisableInterrupts ;//关总中断
  
  ReData = Uart_GetChar(UARTR2);
  sprintf((char*)txt,"UART2_RX: %c \n",ReData);  
  Uart_SendString(UARTR2,txt);
  
  EnableInterrupts;   //开总中断
}

//定时器0中断函数
void PIT0_ISR(void)
{
    DisableInterrupts;                          //关总中断
    PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;//清楚中断标志位  
    read_AD();
    
    EnableInterrupts;            //开总中断
  
}

//定时器1中断函数
void PIT1_ISR(void)
{
    DisableInterrupts;
    PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;//清楚中断标志位
    OLED_BufferClear();
    itoa(adc_fine[0], string2_0);
    itoa(adc_fine[1], string2_1);
    itoa(adc_fine[2], string2_2);
    itoa(adc_fine[3], string2_3);
    itoa(adc_fine[4], string2_4);
    itoa(adc_fine[5], string2_5);
    OLED_DrawString8X16(30,42,string2_0,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30,21,string2_1,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30,0,string2_2,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(0,42,string2_6,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(0,21,string2_7,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(0,0,string2_8,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(OLED_DRAW_WIDTH_MAX/2,42,string2_9,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(OLED_DRAW_WIDTH_MAX/2,21,string2_10,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(OLED_DRAW_WIDTH_MAX/2,0,string2_11,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30+OLED_DRAW_WIDTH_MAX/2,42,string2_3,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30+OLED_DRAW_WIDTH_MAX/2,21,string2_4,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_DrawString8X16(30+OLED_DRAW_WIDTH_MAX/2,0,string2_5,OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
    OLED_BufferFlash();
 /*   EventCount++; 
    int test;
    if( EventCount == 1200)
      EventCount = 0;
    test = EventCount / 400;
    
    //adc_test(); //测试用 采集电感值到shuju
    Date_analyse(); //提线算法
    if(EventCount%4==0)
    {
      Steer_conter(error1);
    }
 //FTM_PWM_Duty(CFTM2, FTM_CH0, 6500);
 // FTM_PWM_Duty(CFTM2, FTM_CH2, 6500);
     flag_received =1; 
    */
  EnableInterrupts; 
}



//KBI0中断函数
void KBI0_Isr(void)	
{  
  KBI0->SC |= KBI_SC_KBACK_MASK;       /* clear interrupt flag */
  uint16_t n = PTn(KBI_PTB5) ;   //PTA0引脚触发中断 
  if(KBI0->SP &(1<<n))
  {
    //用户代码 
    LED_Ctrl(LED0, LEDRVS);             
  } 
}

//KBI1中断函数
void KBI1_Isr(void)	
{  
  KBI1->SC |= KBI_SC_KBACK_MASK;                /* clear interrupt flag */
  
  uint16_t n = PTn(KBI_PTH2) ;   //PTH2引脚触发中断 
  if(KBI1->SP &(1<<n))
  {
    //用户代码 
    LED_Ctrl(LED1, LEDRVS);             
  }
}


/*****************************************************************************//*!
*
* @brief  FTM0_Isr interrupt service routine.
*        
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void FTM0_IRQHandler(void)
{
  /* clear the flag */
  FTMx[CFTM0]->SC &= ~FTM_SC_TOF_MASK;
  
}

/*****************************************************************************//*!
*
* @brief  FTM1_Isr interrupt service routine.
*        
* @param  none. 
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void FTM1_IRQHandler(void)
{
  /* clear the flag */
  FTMx[CFTM1]->SC &= ~FTM_SC_TOF_MASK;
}

/*****************************************************************************//*!
*
* @brief  FTM2_Isr interrupt service routine.
*        
* @param  none. 
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void FTM2_IRQHandler(void)
{
  /* clear the flag */
  FTMx[CFTM2]->SC &= ~FTM_SC_TOF_MASK;
}
