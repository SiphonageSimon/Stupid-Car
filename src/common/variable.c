#include"include.h"

uint16_t adc_res[6][AD_BUFFER] = {{0}}; //�Զ������й�һ�����ֵ
uint8_t  adc_res_pos = 0;               //����λ��ָʾ
uint16_t adc_fine[6] = {0};             //�����˲����������
unsigned short OutData[4] = {0};
uint8_t flag_received = 0;
uint8_t current_State = FSM_INIT;

//oled��ʾ�ַ���
  unsigned char string0[] = "Initializing...";
  unsigned char string0_1[] = "Using Pin E6";
  unsigned char string1[] = "Value1= ";
  unsigned char string2_0[20];
  unsigned char string2_1[20];
  unsigned char string2_2[20];
  unsigned char string2_3[20];
  unsigned char string2_4[20];
  unsigned char string2_5[20];
  unsigned char string2_6[] = "L1:";
  unsigned char string2_7[] = "L2:";
  unsigned char string2_8[] = "L3:";
  unsigned char string2_9[] = "L4:";
  unsigned char string2_10[] = "L5:";
  unsigned char string2_11[] = "L6:";
  void itoa(int i, char *string) //itoa����ʵ��
{
        int power=0,j=0;
        if(i < 0)
        {
          *string++='-';
          i = -i;
        }
        j=i;
        for( power=1;j>10;j/=10)
                power*=10;
 
        for(;power>0;power/=10)
        {
                *string++='0'+i/power;
                i%=power;
        }
        *string='\0';
}

