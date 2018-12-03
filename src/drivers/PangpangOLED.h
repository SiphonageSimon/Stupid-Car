#include "include.h"
//∞Ê±æ∫≈£∫V1.1.0
//Õ¯  ÷∑£∫http://idown.win
//¡™  œµ£∫zhenwenjin@gmail.com

#ifndef PANGPANGOLED_H
#define PANGPANGOLED_H		  	 

#define OLED_SCL 			PTF3									//IIC≤„–Ë≈‰÷√
#define OLED_SDA 			PTF2									//IIC≤„–Ë≈‰÷√
#define OLED_SYS_BIT_COUNT              unsigned long

#define OLED_SCL_INIT                   gpio_init(OLED_SCL,1,1);//IIC≤„–Ë≈‰÷√
#define OLED_SDA_INIT                   gpio_init(OLED_SDA,1,1);//IIC≤„–Ë≈‰÷√
#define OLED_SCL_H                      gpio_set(OLED_SCL, 1);//IIC≤„–Ë≈‰÷√
#define OLED_SCL_L                      gpio_set(OLED_SCL, 0);//IIC≤„–Ë≈‰÷√
#define OLED_SDA_H                      gpio_set(OLED_SDA, 1);//IIC≤„–Ë≈‰÷√
#define OLED_SDA_L                      gpio_set(OLED_SDA, 0);//IIC≤„–Ë≈‰÷√
#define OLED_DELAY_MS(inputTimes)	time_delay_ms(inputTimes);      //IIC≤„–Ë≈‰÷√

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

#define OLED_GUI_WINDOW_NUMBER_MAX      8
#define OLED_GUI_HOME_WINDOW_NUMBER     0
#define OLED_GUI_TITLE_CHAR_HEIGHT      16
#define OLED_GUI_TITLE_CHAR_WIDTH       8
#define OLED_GUI_PLANE_MAX_Y            (OLED_DRAW_HEIGHT_MAX-OLED_GUI_TITLE_CHAR_HEIGHT-1)
#define OLED_GUI_PLANE_MAX_X            OLED_DRAW_WIDTH_MAX
#define OLED_GUI_PLANE_BYTE_SIZE        (OLED_WIDTH*OLED_HEIGHT/8)//(unsigned int)((((OLED_GUI_PLANE_MAX_Y+1)*(OLED_GUI_PLANE_MAX_X+1))/8)+1)     //+1∑¿÷π8”‡ ˝
#define OLED_GUI_KEY_UP_CLICK           0
#define OLED_GUI_KEY_LEFT_CLICK         1
#define OLED_GUI_KEY_CENTRE_CLICK       2
#define OLED_GUI_KEY_HOME_CLICK         3
#define OLED_GUI_KEY_RIGHT_CLICK        4
#define OLED_GUI_KEY_DOWN_CLICK         5  

void OLED_IICInit(void);
void OLED_IICStart(void);
void OLED_IICStop(void);
void OLED_IICWaitAck(void);
void OLED_IICWriteByte(unsigned char writeByte);
void OLED_IICWriteCmd(unsigned char writeCmd);
void OLED_IICWriteData(unsigned char writeData);

void OLED_Init(void);
void OLED_PosSet(unsigned char x,unsigned char y);
void OLED_ByteWrite(unsigned char writeByte);
void OLED_ByteSet(unsigned char x,unsigned char y,unsigned char byte);

void OLED_BufferClear(void);
void OLED_BufferFill(void);
void OLED_BufferPointSet(unsigned char x,unsigned char y,unsigned char setBit);
void OLED_BufferByteSet(unsigned char x,unsigned char y,unsigned char setByte);
unsigned char OLED_BufferByteGet(unsigned char x,unsigned char y);
void OLED_BufferFlashByFlag(void);
void OLED_BufferFlashAll(void);
void OLED_BufferFlash(void);
void OLED_BufferFlashFlasgClear(void);
void OLED_BufferFlashFlagSet(unsigned char x,unsigned char y,unsigned char setBool);
unsigned char OLED_BufferFlashFlagGet(unsigned char x,unsigned char y);

void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char color);
void OLED_DrawLine(unsigned char startX,unsigned char startY,unsigned char endX,unsigned char endY,unsigned char color);
void OLED_DrawCircle(unsigned char centerX,unsigned char centerY,unsigned char radius,unsigned char color,unsigned char fillBool);
void OLED_DrawRectangle(unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char color,unsigned char fillBool);
void OLED_DrawTriangle(unsigned char upX,unsigned char upY,unsigned char leftX,unsigned char ledftY,unsigned char rightX,unsigned char rightY,unsigned char color);
void OLED_DrawRoundedRectangle(unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char radius,unsigned char color);
void OLED_DrawChar8X16(unsigned char x,unsigned char y,unsigned char drawChar,unsigned char color,unsigned char fillBool,unsigned char angle);
void OLED_DrawString8X16(unsigned char x,unsigned char y,unsigned char * drawString,unsigned char color,unsigned char fillBool,unsigned char angle,unsigned uprightBool);

void OLED_GUIFlash(void);
unsigned char * OLED_GUIWindowTitleGet(unsigned char windowNumber);
void OLED_GUIWindowTitleSet(unsigned char windowNumber,unsigned char * setTitle);
unsigned char OLED_GUIFocusWindowFlagGet(void);
void OLED_GUIFocusWindowFlagSet(unsigned char windowNumber);
unsigned char OLED_GUIEnableWindowFlagGet(unsigned char windowNumber);
void OLED_GUIEnableWindowFlagSet(unsigned char windowNumber,unsigned char setBit);
void OLED_GUIWindowAdd(unsigned char windowNumber,unsigned char * setTitle,OLED_SYS_BIT_COUNT setWindowInitAddress,unsigned char * setWindowPlaneAddress,OLED_SYS_BIT_COUNT setKeyHandleAddress);
void OLED_GUIInit(unsigned char * setHomeWindowTitle,OLED_SYS_BIT_COUNT setWindowInitAddress,unsigned char * setWindowPlaneAddress);

void OLED_GUIPlanePointSet(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char setBit);
unsigned char OLED_GUIPlanePointGet(unsigned char windowNumber,unsigned char x,unsigned char y);
void OLED_GUIPlaneClear(unsigned char windowNumber);
void OLED_GUIPlaneFill(unsigned char windowNumber);

void OLED_GUIPlaneDrawPoint(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char color);
void OLED_GUIPlaneDrawLine(unsigned char windowNumber,unsigned char startX,unsigned char startY,unsigned char endX,unsigned char endY,unsigned char color);
void OLED_GUIPlaneDrawCircle(unsigned char windowNumber,unsigned char centerX,unsigned char centerY,unsigned char radius,unsigned char color,unsigned char fillBool);
void OLED_GUIPlaneDrawRectangle(unsigned char windowNumber,unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char color,unsigned char fillBool);
void OLED_GUIPlaneDrawTriangle(unsigned char windowNumber,unsigned char upX,unsigned char upY,unsigned char leftX,unsigned char leftY,unsigned char rightX,unsigned char rightY,unsigned char color);
void OLED_GUIPlaneDrawRoundedRectangle(unsigned char windowNumber,unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char radius,unsigned char color);
void OLED_GUIPlaneDrawChar8X16(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char GUIPlaneDrawChar,unsigned char color,unsigned char fillBool,unsigned char angle);
void OLED_GUIPlaneDrawString8X16(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char * GUIPlaneDrawString,unsigned char color,unsigned char fillBool,unsigned char angle,unsigned uprightBool);

void OLED_GUIKeyHandleHome(unsigned char windowNumber,unsigned char inputKey);
void OLED_GUIKeyInput(unsigned char inputKey);

#endif  
	 