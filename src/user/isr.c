#include "include.h"
int UART4_FLAG=0;
int EventCount=0,ti=0,in=0;
//����0�����жϷ�������
void UART0_ISR(void)
{
  uint8_t ReData;
  uint8_t txt[20];
  
  DisableInterrupts ;//�����ж�
  
  ReData = Uart_GetChar(UARTR0);
  sprintf((char*)txt,"UART0_RX: %c \n",ReData);  
  Uart_SendString(UARTR0,txt);
  
  EnableInterrupts;   //�����ж�
}


//����1�����жϷ�������
void UART1_ISR(void)
{
  uint8_t ReData;
  uint8_t txt[20];
  
  DisableInterrupts ;//�����ж�
  
  ReData = Uart_GetChar(UARTR1);
  sprintf((char*)txt,"UART1_RX: %c \n",ReData);  
  Uart_SendString(UARTR1,txt);
  
  EnableInterrupts;   //�����ж�
}

//����2�����жϷ�������
void UART2_ISR(void)
{
  uint8_t ReData;
  uint8_t txt[20];
  
  DisableInterrupts ;//�����ж�
  
  ReData = Uart_GetChar(UARTR2);
  sprintf((char*)txt,"UART2_RX: %c \n",ReData);  
  Uart_SendString(UARTR2,txt);
  
  EnableInterrupts;   //�����ж�
}

//��ʱ��0�жϺ���
void PIT0_ISR(void)
{
    DisableInterrupts;                          //�����ж�
    PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;//����жϱ�־λ  
    read_AD(); //��ȡadcֵ���˲��͹�һ��
    EnableInterrupts;            //�����ж�
  
}

//��ʱ��1�жϺ���
void PIT1_ISR(void)
{
    DisableInterrupts;
    PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;//����жϱ�־λ
#if ADC_VAL_DSPLY    //��������еĶ�����ʾ��OLED���ϣ�ADC_VAL_DSPLYλ��valuable.h
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
#endif
    servo_Ctrl(); //������ƣ���������
    EnableInterrupts; 
}



//KBI0�жϺ���
void KBI0_Isr(void)	
{  
  KBI0->SC |= KBI_SC_KBACK_MASK;       /* clear interrupt flag */
  uint16_t n = PTn(KBI_PTB5) ;   //PTA0���Ŵ����ж� 
  if(KBI0->SP &(1<<n))
  {
    //�û����� 
    LED_Ctrl(LED0, LEDRVS);             
  } 
}

//KBI1�жϺ���
void KBI1_Isr(void)	
{  
  KBI1->SC |= KBI_SC_KBACK_MASK;                /* clear interrupt flag */
  
  uint16_t n = PTn(KBI_PTH2) ;   //PTH2���Ŵ����ж� 
  if(KBI1->SP &(1<<n))
  {
    //�û����� 
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

