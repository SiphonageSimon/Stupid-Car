#pragma once

//������غ�
#define INIT_DSPLY          1            //main���Ƿ���ʾ��ʼ����ʾ��
#define STATE_VAL_DISPLY    1            //main���Ƿ���ʾ��ж����͵�ǰ״̬
#define SCOPE_SEND          1            //main���Ƿ�������ʾ����ֵ
#define ADC_VAL_DSPLY       0            //isr1���Ƿ���ʾ�����ֵ
#define LINEAR_TEST         0            //FSM_Ctrl���Ƿ�����������Ի�����


//adc�ɼ�����
#define AD_BUFFER 20                     //�����˲�������
#define AD_AVETIME 10                    //��ֵ��������
#define AD_VALUE_1 1000                  //��һ������
#define AD_ALPHA 0.8                     //�����˲�Ȩ��

//adcͨ��ѡ��
#define ADC0 ADC_CHANNEL_AD4             //PTB0
#define ADC1 ADC_CHANNEL_AD5             //PTB1
#define ADC2 ADC_CHANNEL_AD12            //PTF4
#define ADC3 ADC_CHANNEL_AD8             //PTC0
#define ADC4 ADC_CHANNEL_AD13            //PTF5
#define ADC5 ADC_CHANNEL_AD14            //PTF6

//����Ϊ״̬��״̬��Χ
#define FSM_INIT 0                       //��ʼ��״̬
#define FSM_STRAIGHT 1                   //֮��
#define FSM_CROSSROAD 2                  //ʮ��·��
#define FSM_LEFT_CORNER 3                //�����
#define FSM_RIGHT_CORNER 4               //�����
#define FSM_S_TURN 5                     //������
#define FSM_RAMP 6                       //�µ�������
#define FSM_RAMPTOP 7                    //�µ���������
#define FSM_GOAL 8                       //�յ�
#define FSM_OUT_OF_COURSE 255            //���磬ռλ


extern uint8_t current_State;           //״̬����ǰ״̬
extern uint16_t adc_res[6][AD_BUFFER];  //�ڲ�������������adc.c�����޸�����ֵ
extern uint8_t  adc_res_pos;            //�ڲ�������������adc.c�����޸�����ֵ
extern uint16_t adc_fine[6];            //��һ���͹����˲����ֵ������ʹ�ã�������adc.c�����޸�����ֵ
extern uint8_t flag_received;           //uart����ָʾλ
extern unsigned short OutData[4];     //����ʾ���������ֵ

//OLED��ʾ����
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

//���ݸ�ʽת������
void itoa(int i, char *string);         //�з���intת�ַ���