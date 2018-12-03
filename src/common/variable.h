#define AD_BUFFER 20    //惯性滤波器长度
#define AD_AVETIME 10   //均值采样次数
#define AD_VALUE_1 1000 //归一化精度
#define AD_ALPHA 0.8
//adc通道选择
#define ADC0 ADC_CHANNEL_AD4        //PTB0
#define ADC1 ADC_CHANNEL_AD5        //PTB1
#define ADC2 ADC_CHANNEL_AD12       //PTF4
#define ADC3 ADC_CHANNEL_AD8        //PTC0
#define ADC4 ADC_CHANNEL_AD13       //PTF5
#define ADC5 ADC_CHANNEL_AD14       //PTF6

extern uint16_t adc_res[6][AD_BUFFER];
extern uint8_t  adc_res_pos;
extern uint16_t adc_fine[6];

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

void itoa(int i, char *string);