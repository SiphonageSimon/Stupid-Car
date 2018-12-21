#include "include.h"

//adc��Ӧ���λ��
#define AD_LEFT 0
#define AD_MID  1
#define AD_RIGHT 3
#define AD_LEFT_VERTICAL 2
#define AD_RIGHT_VERTICAL 4

/*��в��÷���
��LEFT_VERTICAL   ��RIGHT_VERTICAL




�z       �z       �z
LEFT     MID     RIGHT
*/

//״̬�������ת����
typedef struct Tres                     //״̬�л���ֵ�ṹ�壬�����Ǿ��ô�д��������
{
  uint8_t enter_crossroad;              //����ʮ��·��
  uint8_t enter_corner;                 //�������
  uint8_t enter_straight;               //����ֱ��
  uint8_t enter_sturn;                  //����������
  uint8_t no_val;                       //�޶�����Ĭ��Ϊ0
}Threshold;

//�ٶ����
#define SPD_GIVEN                       //��������
#define SPD_GOAL                        //�յ㳵��
//ģ����ز���
#define NB -1000
#define NM -400
#define NS -200
#define ZO 0
#define PS 200
#define PM 400
#define PB 1000

//�û��Զ��庯��
void FSM_Ctrl(void);
void motor_Ctrl(uint16_t left_Spd, uint16_t right_Spd);
void servo_Ctrl(void);
void FSM_select(void);
void fuzzy_Ctrl(void);

