#include "include.h"
#define AD_LEFT 0
#define AD_MID  1
#define AD_RIGHT 3
#define AD_LEFT_VERTICAL 2
#define AD_RIGHT_VERTICAL 4

#define NB -1000
#define NM -400
#define NS -200
#define ZO 0
#define PS 200
#define PM 400
#define PB 1000

//状态机相关跳转参数


//////
void servo_Ctrl(void);
void FSM_select(void);
void fuzzy_Ctrl(void);

//variables about fuzzy control