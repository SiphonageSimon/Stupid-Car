#include "include.h"

//adc对应电感位置
#define AD_LEFT 0
#define AD_MID  1
#define AD_RIGHT 3
#define AD_LEFT_VERTICAL 2
#define AD_RIGHT_VERTICAL 4

/*电感布置方案
▋LEFT_VERTICAL   ▋RIGHT_VERTICAL




▃       ▃       ▃
LEFT     MID     RIGHT
*/

//状态机相关跳转参数
typedef struct Tres                     //状态切换阈值结构体，仅仅是觉得大写看着难受
{
  uint8_t enter_crossroad;              //进入十字路口
  uint8_t enter_corner;                 //进入弯道
  uint8_t enter_straight;               //进入直道
  uint8_t enter_sturn;                  //进入连续弯
  uint8_t no_val;                       //无读数，默认为0
}Threshold;

//速度相关
#define SPD_GIVEN                       //给定车速
#define SPD_GOAL                        //终点车速
//模糊相关参数
#define NB -1000
#define NM -400
#define NS -200
#define ZO 0
#define PS 200
#define PM 400
#define PB 1000

//用户自定义函数
void FSM_Ctrl(void);
void motor_Ctrl(uint16_t left_Spd, uint16_t right_Spd);
void servo_Ctrl(void);
void FSM_select(void);
void fuzzy_Ctrl(void);

