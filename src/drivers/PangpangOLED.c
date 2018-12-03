
//版本号：V1.1.0
//网  址：http://idown.win
//联  系：zhenwenjin@gmail.com

#include "PangpangOLED.h"

void OLED_IICInit(void){
  OLED_SCL_INIT
  OLED_SDA_INIT
  //初始化IIC管脚
}

void OLED_IICStart(void){
  OLED_SCL_H
  OLED_SDA_H
  OLED_SDA_L
  OLED_SCL_L
}

void OLED_IICStop(void){
  OLED_SCL_H
  OLED_SCL_L
  OLED_SDA_L
  OLED_SDA_H
}

void OLED_IICWaitAck(void){
  OLED_SCL_H
  OLED_SCL_L 
}

void OLED_IICWriteByte(unsigned char writeByte){
  unsigned char index;
  OLED_SCL_L
  for(index=0;index<8;index++){
    if(writeByte & 0x80)
      OLED_SDA_H
    else
      OLED_SDA_L
    OLED_SCL_H
    OLED_SCL_L
    writeByte<<=1;
  }
}

void OLED_IICWriteCmd(unsigned char writeCmd){
  OLED_IICStart();
  OLED_IICWriteByte(0x78);
  OLED_IICWaitAck();	
  OLED_IICWriteByte(0x00);
  OLED_IICWaitAck();	
  OLED_IICWriteByte(writeCmd); 
  OLED_IICWaitAck();	
  OLED_IICStop();
}

void OLED_IICWriteData(unsigned char writeData){
  OLED_IICStart();
  OLED_IICWriteByte(0x78);
  OLED_IICWaitAck();	
  OLED_IICWriteByte(0x40);
  OLED_IICWaitAck();	
  OLED_IICWriteByte(writeData); 
  OLED_IICWaitAck();	
  OLED_IICStop();
}
				    
void OLED_Init(void){
  OLED_IICInit();
  
  OLED_DELAY_MS(200);

  OLED_IICWriteCmd(0xAE);//--display off
  
  OLED_IICWriteCmd(0x00);//---set low column address
  OLED_IICWriteCmd(0x10);//---set high column address
  OLED_IICWriteCmd(0x40);//--set start line address  
  OLED_IICWriteCmd(0xB0);//--set page address
  
  OLED_IICWriteCmd(0x81); // contract control
  OLED_IICWriteCmd(0xFF);//--128   
  OLED_IICWriteCmd(0xA1);//set segment remap 
  OLED_IICWriteCmd(0xA6);//--normal / reverse
  OLED_IICWriteCmd(0xA8);//--set multiplex ratio(1 to 64)
  OLED_IICWriteCmd(0x3F);//--1/32 duty
  OLED_IICWriteCmd(0xC8);//Com scan direction
  OLED_IICWriteCmd(0xD3);//-set display offset
  OLED_IICWriteCmd(0x00);//
	
  OLED_IICWriteCmd(0xD5);//set osc division
  OLED_IICWriteCmd(0x80);//
	
  OLED_IICWriteCmd(0xD8);//set area color mode off
  OLED_IICWriteCmd(0x05);//
	
  OLED_IICWriteCmd(0xD9);//Set Pre-Charge Period
  OLED_IICWriteCmd(0xF1);//
	
  OLED_IICWriteCmd(0xDA);//set com pin configuartion
  OLED_IICWriteCmd(0x12);//
	
  OLED_IICWriteCmd(0xDB);//set Vcomh
  OLED_IICWriteCmd(0x30);//
	
  OLED_IICWriteCmd(0x8D);//set charge pump enable
  OLED_IICWriteCmd(0x14);//
	
  OLED_IICWriteCmd(0xAF);//--turn on oled panel
}

void OLED_PosSet(unsigned char x,unsigned char y){
  OLED_IICWriteCmd(0xb0+y);
  OLED_IICWriteCmd(x&0x0f);
  OLED_IICWriteCmd(((x&0xf0)>>4)|0x10);
}

void OLED_ByteWrite(unsigned char writeByte){
  OLED_IICWriteData(writeByte);
}

void OLED_ByteSet(unsigned char x,unsigned char y,unsigned char setByte){
  OLED_PosSet(x,y);
  OLED_ByteWrite(setByte);
}

unsigned char OLED_Buffer[OLED_HEIGHT/8][OLED_WIDTH];
unsigned char OLED_BufferFlashFlag[OLED_BYTE_NUMBER];
unsigned int  OLED_BufferFlashFlagNumber = 0;

void OLED_BufferByteSet(unsigned char x,unsigned char y,unsigned char setByte){
  if(OLED_Buffer[y][x] != setByte){
    OLED_Buffer[y][x] = setByte;
    OLED_BufferFlashFlagSet(x,y,OLED_TRUE);
  }
}

unsigned char OLED_BufferByteGet(unsigned char x,unsigned char y){
  return OLED_Buffer[y][x];
}

void OLED_BufferFlashAll(void){
  unsigned char x,y;
  for(y=0;y<OLED_HEIGHT/8;y++){
    OLED_PosSet(0,y);
    for(x=0;x<OLED_WIDTH;x++){
      OLED_ByteWrite(OLED_BufferByteGet(x,y));
    }
  }
}

void OLED_BufferFlash(void){
  if(OLED_BufferFlashFlagNumber>OLED_FLASH_FLAG_BOUNDARY){
    OLED_BufferFlashAll();
  }
  else{
    OLED_BufferFlashByFlag();
  }
  OLED_BufferFlashFlasgClear();
}

void OLED_BufferClear(void){
  unsigned char x,y;
  for(y=0;y<OLED_HEIGHT/8;y++){
    for(x=0;x<OLED_WIDTH;x++){
      OLED_BufferByteSet(x,y,0x00);
    }
  }
}

void OLED_BufferFill(void){
  unsigned char x,y;
  for(y=0;y<OLED_HEIGHT/8;y++){
    for(x=0;x<OLED_WIDTH;x++){
      OLED_BufferByteSet(x,y,0xff);
    }
  }
}

void OLED_BufferPointSet(unsigned char x,unsigned char y,unsigned char setBit){
  if(OLED_SCREEN_FLIPPING == OLED_TRUE){
    x=OLED_DRAW_WIDTH_MAX-x;
    y=OLED_DRAW_HEIGHT_MAX-y;
  }
  if(setBit){
    OLED_BufferByteSet(x,y/8,OLED_Buffer[(y/8)][x]|(0x1<<(y%8)));
  }
  else{
    OLED_BufferByteSet(x,y/8,OLED_Buffer[(y/8)][x]&(~(0x1<<(y%8))));
  }
}

void OLED_BufferFlashByFlag(void){
  unsigned char x,y;
  for(y=0;y<OLED_HEIGHT/8;y++){
    for(x=0;x<OLED_WIDTH;x++){
      if(OLED_BufferFlashFlagGet(x,y) == OLED_TRUE){
        OLED_ByteSet(x,y,OLED_BufferByteGet(x,y));
      }
    }
  }
}

void OLED_BufferFlashFlagSet(unsigned char x,unsigned char y,unsigned char setBool){
  if(setBool){
    OLED_BufferFlashFlag[x] |= (0x1<<y);
    OLED_BufferFlashFlagNumber++;
  }
  else{
    OLED_BufferFlashFlag[x] &= ~(0x1<<y);
    OLED_BufferFlashFlagNumber--;
  }
}

unsigned char OLED_BufferFlashFlagGet(unsigned char x,unsigned char y){
  return (OLED_BufferFlashFlag[x]>>y)&0x1;
}

void OLED_BufferFlashFlasgClear(void){
  unsigned char index;
  for(index=0;index<OLED_BYTE_NUMBER;index++){
    OLED_BufferFlashFlag[index] = 0x00;
  }
  OLED_BufferFlashFlagNumber = 0;
}

void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char color){
  OLED_BufferPointSet(x,OLED_DRAW_HEIGHT_MAX-y,color);
  //进行坐标转换 左下角是(0,0)
}

void OLED_DrawLine(unsigned char startX,unsigned char startY,unsigned char endX,unsigned char endY,unsigned char color){
  unsigned char x,y,temp;
  if(startX>endX){
    temp = startX;
    startX = endX;
    endX = temp;
    temp = startY;
    startY = endY;
    endY = temp;
  }
  if(startX == endX){
    if(startY>endY){
      temp = startY;
      startY = endY;
      endY = temp;
    }
    for(y=startY;y<=endY;y++){
      OLED_DrawPoint(startX,y,color);
    }
  }
  else{
    for(x=startX;x<=endX;x++){
      y = (x-startX)*(endY-startY)/(endX-startX)+startY;
      OLED_DrawPoint(x,y,color);
    }
  }
}

void OLED_DrawCircle(unsigned char centerX,unsigned char centerY,unsigned char radius,unsigned char color,unsigned char fillBool){
  unsigned char x,y;
  for(x=0;x<radius;x++){
    y = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(x*x)));
    if(centerY-y<0){
      y=centerY;
      //预防浮点计算
    }
    OLED_DrawPoint(centerX+x,centerY+y,color);
    //右上
    OLED_DrawPoint(centerX-x,centerY+y,color);
    //左上
    OLED_DrawPoint(centerX+x,centerY-y,color);
    //右下
    OLED_DrawPoint(centerX-x,centerY-y,color);
    //左下
    
    if(fillBool == OLED_TRUE){
      OLED_DrawLine(centerX+x,centerY-y,centerX+x,centerY+y,color);
      //右上下
      OLED_DrawLine(centerX-x,centerY-y,centerX-x,centerY+y,color);
      //左上下
    }
  }
  for(y=0;y<radius;y++){
    x = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(y*y)));
    OLED_DrawPoint(centerX+x,centerY+y,color);
    //右上
    OLED_DrawPoint(centerX-x,centerY+y,color);
    //左上
    OLED_DrawPoint(centerX+x,centerY-y,color);
    //右下
    OLED_DrawPoint(centerX-x,centerY-y,color);
    //左下
  }
}

void OLED_DrawRectangle(unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char color,unsigned char fillBool){
  OLED_DrawLine(startX,startY,startX+width-1,startY,color);
  //底边
  OLED_DrawLine(startX,startY+height-1,startX,startY,color);
  //左边
  OLED_DrawLine(startX,startY+height-1,startX+width-1,startY+height-1,color);
  //顶边
  OLED_DrawLine(startX+width-1,startY+height-1,startX+width-1,startY,color);
  //右边
  
  if(fillBool == OLED_TRUE){
    unsigned char x;
    for(x=startX;x<startX+width;x++){
      OLED_DrawLine(x,startY,x,startY+height-1,color);
    }
  }
}

void OLED_DrawTriangle(unsigned char upX,unsigned char upY,unsigned char leftX,unsigned char leftY,unsigned char rightX,unsigned char rightY,unsigned char color){
  OLED_DrawLine(leftX,leftY,rightX,rightY,color);
  //底边
  OLED_DrawLine(leftX,leftY,upX,upY,color);
  //左边
  OLED_DrawLine(rightX,rightY,upX,upY,color);
  //右边
}

void OLED_DrawRoundedRectangle(unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char radius,unsigned char color){
  unsigned char x,y;
  OLED_DrawLine(startX+radius,startY,startX+width-1-radius,startY,color);
  //底边
  OLED_DrawLine(startX+radius,startY+height-1,startX+width-1-radius,startY+height-1,color);
  //顶边
  OLED_DrawLine(startX,startY+height-1-radius,startX,startY+radius,color);
  //左边
  OLED_DrawLine(startX+width-1,startY+height-1-radius,startX+width-1,startY+radius,color);
  //右边
 
  for(x=0;x<radius;x++){
    y = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(x*x)));
    OLED_DrawPoint(startX+width-1-radius+x,startY+height-1-radius+y,color);
    //右上
    OLED_DrawPoint(startX+radius-x,startY+height-1-radius+y,color);
    //左上
    OLED_DrawPoint(startX+width-1-radius+x,startY+radius-y,color);
    //右下
    OLED_DrawPoint(startX+radius-x,startY+radius-y,color);
    //左下
  }
  for(y=0;y<radius;y++){
    x = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(y*y)));
    OLED_DrawPoint(startX+width-1-radius+x,startY+height-1-radius+y,color);
    //右上
    OLED_DrawPoint(startX+radius-x,startY+height-1-radius+y,color);
    //左上
    OLED_DrawPoint(startX+width-1-radius+x,startY+radius-y,color);
    //右下
    OLED_DrawPoint(startX+radius-x,startY+radius-y,color);
    //左下
  }
}

const unsigned char F8X16[95][16] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x18,0x18,0x00,0x00},/*"!",1*/
{0x00,0x48,0x6C,0x24,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",2*/
{0x00,0x00,0x00,0x24,0x24,0x24,0x7F,0x12,0x12,0x12,0x7F,0x12,0x12,0x12,0x00,0x00},/*"#",3*/
{0x00,0x00,0x08,0x1C,0x2A,0x2A,0x0A,0x0C,0x18,0x28,0x28,0x2A,0x2A,0x1C,0x08,0x08},/*"$",4*/
{0x00,0x00,0x00,0x22,0x25,0x15,0x15,0x15,0x2A,0x58,0x54,0x54,0x54,0x22,0x00,0x00},/*"%",5*/
{0x00,0x00,0x00,0x0C,0x12,0x12,0x12,0x0A,0x76,0x25,0x29,0x11,0x91,0x6E,0x00,0x00},/*"&",6*/
{0x00,0x06,0x06,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/
{0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00},/*"(",8*/
{0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00},/*")",9*/
{0x00,0x00,0x00,0x00,0x08,0x08,0x6B,0x1C,0x1C,0x6B,0x08,0x08,0x00,0x00,0x00,0x00},/*"*",10*/
{0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x7F,0x08,0x08,0x08,0x08,0x00,0x00,0x00},/*"+",11*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x04,0x03},/*",",12*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x00,0x00},/*".",14*/
{0x00,0x00,0x80,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x00},/*"/",15*/
{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",16*/
{0x00,0x00,0x00,0x08,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"1",17*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x20,0x20,0x10,0x08,0x04,0x42,0x7E,0x00,0x00},/*"2",18*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x20,0x18,0x20,0x40,0x40,0x42,0x22,0x1C,0x00,0x00},/*"3",19*/
{0x00,0x00,0x00,0x20,0x30,0x28,0x24,0x24,0x22,0x22,0x7E,0x20,0x20,0x78,0x00,0x00},/*"4",20*/
{0x00,0x00,0x00,0x7E,0x02,0x02,0x02,0x1A,0x26,0x40,0x40,0x42,0x22,0x1C,0x00,0x00},/*"5",21*/
{0x00,0x00,0x00,0x38,0x24,0x02,0x02,0x1A,0x26,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"6",22*/
{0x00,0x00,0x00,0x7E,0x22,0x22,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},/*"7",23*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",24*/
{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x64,0x58,0x40,0x40,0x24,0x1C,0x00,0x00},/*"9",25*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00},/*":",26*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x04},/*";",27*/
{0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00},/*"<",28*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},/*"=",29*/
{0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00},/*">",30*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x46,0x40,0x20,0x10,0x10,0x00,0x18,0x18,0x00,0x00},/*"?",31*/
{0x00,0x00,0x00,0x1C,0x22,0x5A,0x55,0x55,0x55,0x55,0x2D,0x42,0x22,0x1C,0x00,0x00},/*"@",32*/
{0x00,0x00,0x00,0x08,0x08,0x18,0x14,0x14,0x24,0x3C,0x22,0x42,0x42,0xE7,0x00,0x00},/*"A",33*/
{0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x1E,0x22,0x42,0x42,0x42,0x22,0x1F,0x00,0x00},/*"B",34*/
{0x00,0x00,0x00,0x7C,0x42,0x42,0x01,0x01,0x01,0x01,0x01,0x42,0x22,0x1C,0x00,0x00},/*"C",35*/
{0x00,0x00,0x00,0x1F,0x22,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1F,0x00,0x00},/*"D",36*/
{0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x42,0x42,0x3F,0x00,0x00},/*"E",37*/
{0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x02,0x02,0x07,0x00,0x00},/*"F",38*/
{0x00,0x00,0x00,0x3C,0x22,0x22,0x01,0x01,0x01,0x71,0x21,0x22,0x22,0x1C,0x00,0x00},/*"G",39*/
{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"H",40*/
{0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"I",41*/
{0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x11,0x0F},/*"J",42*/
{0x00,0x00,0x00,0x77,0x22,0x12,0x0A,0x0E,0x0A,0x12,0x12,0x22,0x22,0x77,0x00,0x00},/*"K",43*/
{0x00,0x00,0x00,0x07,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x7F,0x00,0x00},/*"L",44*/
{0x00,0x00,0x00,0x77,0x36,0x36,0x36,0x36,0x2A,0x2A,0x2A,0x2A,0x2A,0x6B,0x00,0x00},/*"M",45*/
{0x00,0x00,0x00,0xE3,0x46,0x46,0x4A,0x4A,0x52,0x52,0x52,0x62,0x62,0x47,0x00,0x00},/*"N",46*/
{0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1C,0x00,0x00},/*"O",47*/
{0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x42,0x3E,0x02,0x02,0x02,0x02,0x07,0x00,0x00},/*"P",48*/
{0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x4D,0x53,0x32,0x1C,0x60,0x00},/*"Q",49*/
{0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x3E,0x12,0x12,0x22,0x22,0x42,0xC7,0x00,0x00},/*"R",50*/
{0x00,0x00,0x00,0x7C,0x42,0x42,0x02,0x04,0x18,0x20,0x40,0x42,0x42,0x3E,0x00,0x00},/*"S",51*/
{0x00,0x00,0x00,0x7F,0x49,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00},/*"T",52*/
{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"U",53*/
{0x00,0x00,0x00,0xE7,0x42,0x42,0x22,0x24,0x24,0x14,0x14,0x18,0x08,0x08,0x00,0x00},/*"V",54*/
{0x00,0x00,0x00,0x6B,0x49,0x49,0x49,0x49,0x55,0x55,0x36,0x22,0x22,0x22,0x00,0x00},/*"W",55*/
{0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00},/*"X",56*/
{0x00,0x00,0x00,0x77,0x22,0x22,0x14,0x14,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00},/*"Y",57*/
{0x00,0x00,0x00,0x7E,0x21,0x20,0x10,0x10,0x08,0x04,0x04,0x42,0x42,0x3F,0x00,0x00},/*"Z",58*/
{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00},/*"[",59*/
{0x00,0x00,0x02,0x02,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x20,0x20,0x20,0x40,0x40},/*"\",60*/
{0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00},/*"]",61*/
{0x00,0x38,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},/*"_",63*/
{0x00,0x06,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x78,0x44,0x42,0x42,0xFC,0x00,0x00},/*"a",65*/
{0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x1A,0x26,0x42,0x42,0x42,0x26,0x1A,0x00,0x00},/*"b",66*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x02,0x02,0x02,0x44,0x38,0x00,0x00},/*"c",67*/
{0x00,0x00,0x00,0x60,0x40,0x40,0x40,0x78,0x44,0x42,0x42,0x42,0x64,0xD8,0x00,0x00},/*"d",68*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x7E,0x02,0x02,0x42,0x3C,0x00,0x00},/*"e",69*/
{0x00,0x00,0x00,0xF0,0x88,0x08,0x08,0x7E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"f",70*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x22,0x22,0x1C,0x02,0x3C,0x42,0x42,0x3C},/*"g",71*/
{0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"h",72*/
{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x0E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"i",73*/
{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x38,0x20,0x20,0x20,0x20,0x20,0x20,0x22,0x1E},/*"j",74*/
{0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x72,0x12,0x0A,0x16,0x12,0x22,0x77,0x00,0x00},/*"k",75*/
{0x00,0x00,0x00,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"l",76*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x92,0x92,0x92,0x92,0x92,0xB7,0x00,0x00},/*"m",77*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"n",78*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"o",79*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x26,0x42,0x42,0x42,0x22,0x1E,0x02,0x07},/*"p",80*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x44,0x42,0x42,0x42,0x44,0x78,0x40,0xE0},/*"q",81*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x4C,0x04,0x04,0x04,0x04,0x1F,0x00,0x00},/*"r",82*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x42,0x02,0x3C,0x40,0x42,0x3E,0x00,0x00},/*"s",83*/
{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x3E,0x08,0x08,0x08,0x08,0x08,0x30,0x00,0x00},/*"t",84*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x42,0x42,0x42,0x42,0x62,0xDC,0x00,0x00},/*"u",85*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x14,0x08,0x08,0x00,0x00},/*"v",86*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEB,0x49,0x49,0x55,0x55,0x22,0x22,0x00,0x00},/*"w",87*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x24,0x18,0x18,0x18,0x24,0x6E,0x00,0x00},/*"x",88*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x14,0x18,0x08,0x08,0x07},/*"y",89*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x22,0x10,0x08,0x08,0x44,0x7E,0x00,0x00},/*"z",90*/
{0x00,0xC0,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0xC0,0x00},/*"{",91*/
{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},/*"|",92*/
{0x00,0x06,0x08,0x08,0x08,0x08,0x08,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x06,0x00},/*"}",93*/
{0x0C,0x32,0xC2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",94*/
};

void OLED_DrawChar8X16(unsigned char x,unsigned char y,unsigned char drawChar,unsigned char color,unsigned char fillBool,unsigned char angle){
  unsigned char xPos,yPos;
  if(angle == OLED_ANGLE_0){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[drawChar-' '][15-yPos]>>xPos)&0x01 != 0){
          OLED_DrawPoint(x+xPos,y+yPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_DrawPoint(x+xPos,y+yPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_DrawPoint(x+xPos,y+yPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
  else if(angle == OLED_ANGLE_90){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[drawChar-' '][yPos]>>xPos)&0x01 != 0){
          OLED_DrawPoint(x+yPos,y+xPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_DrawPoint(x+yPos,y+xPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_DrawPoint(x+yPos,y+xPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
  else if(angle == OLED_ANGLE_180){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[drawChar-' '][yPos]>>xPos)&0x01 != 0){
          OLED_DrawPoint(x+7-xPos,y+yPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_DrawPoint(x+7-xPos,y+yPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_DrawPoint(x+7-xPos,y+yPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
  else if(angle == OLED_ANGLE_270){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[drawChar-' '][15-yPos]>>(7-xPos))&0x01 != 0){
          OLED_DrawPoint(x+yPos,y+xPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_DrawPoint(x+yPos,y+xPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_DrawPoint(x+yPos,y+xPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
}

void OLED_DrawString8X16(unsigned char x,unsigned char y,unsigned char * drawString,unsigned char color,unsigned char fillBool,unsigned char angle,unsigned uprightBool){
  while(*drawString != '\0'){
    OLED_DrawChar8X16(x,y,*drawString,color,fillBool,angle);
    if(uprightBool == OLED_FALSE){
      if(angle == OLED_ANGLE_0){
        x+=8;
      }
      else if(angle == OLED_ANGLE_90){
        y+=8;
      }
      else if(angle == OLED_ANGLE_180){
        x-=8;
      }
      else if(angle == OLED_ANGLE_270){
        y-=8;
      }
    }
    else{
      if(angle == OLED_ANGLE_0){
        y-=16;
      }
      else if(angle == OLED_ANGLE_90){
        x+=16;
      }
      else if(angle == OLED_ANGLE_180){
        y+=16;
      }
      else if(angle == OLED_ANGLE_270){
        x-=16;
      }
    }
    drawString++;
  }
}

unsigned char   OLED_GUIFocusWindowFlag[OLED_GUI_WINDOW_NUMBER_MAX/8];
unsigned char   OLED_GUIEnableWindowFlag[OLED_GUI_WINDOW_NUMBER_MAX/8];
unsigned char * OLED_GUIWindowTitleAddress[OLED_GUI_WINDOW_NUMBER_MAX];
OLED_SYS_BIT_COUNT OLED_GUIWindowInitAddress[OLED_GUI_WINDOW_NUMBER_MAX];
unsigned char * OLED_GUIWindowPlaneAddress[OLED_GUI_WINDOW_NUMBER_MAX];
OLED_SYS_BIT_COUNT OLED_GUIKeyHandleAddress[OLED_GUI_WINDOW_NUMBER_MAX];

void OLED_GUIEnableWindowFlagSet(unsigned char windowNumber,unsigned char setBit){
  unsigned char index,indexBit;
  for(index=OLED_GUI_WINDOW_NUMBER_MAX/8-1;index<OLED_GUI_WINDOW_NUMBER_MAX/8;index++){
    for(indexBit=0;indexBit<8;indexBit++){
      if(index/8+indexBit == windowNumber){
        if(setBit){
          OLED_GUIEnableWindowFlag[index] |=(0x01<<indexBit);
        }
        else{
          OLED_GUIEnableWindowFlag[index] &=~(0x01<<indexBit);
        }
      }
    }
  }
}

unsigned char OLED_GUIEnableWindowFlagGet(unsigned char windowNumber){
  unsigned char index,indexBit;
  for(index=OLED_GUI_WINDOW_NUMBER_MAX/8-1;index<OLED_GUI_WINDOW_NUMBER_MAX/8;index++){
    for(indexBit=0;indexBit<8;indexBit++){
      if(index/8+indexBit == windowNumber){
        return ((OLED_GUIEnableWindowFlag[index]>>indexBit)&0x01);
      }
    }
  }
  return 0;
}

void OLED_GUIFocusWindowFlagSet(unsigned char windowNumber){
  unsigned char index,indexBit;
  for(index=OLED_GUI_WINDOW_NUMBER_MAX/8-1;index<OLED_GUI_WINDOW_NUMBER_MAX/8;index++){
    for(indexBit=0;indexBit<8;indexBit++){
      if(index/8+indexBit == windowNumber){
        OLED_GUIFocusWindowFlag[index] |=(0x01<<indexBit);
      }
      else{
        OLED_GUIFocusWindowFlag[index] &=~(0x01<<indexBit);
      }
    }
  }
}

unsigned char OLED_GUIFocusWindowFlagGet(void){
  unsigned char index,indexBit,indexCount = 0;
  for(index=OLED_GUI_WINDOW_NUMBER_MAX/8-1;index<OLED_GUI_WINDOW_NUMBER_MAX/8;index++){
    for(indexBit=0;indexBit<8;indexBit++){
      if((OLED_GUIFocusWindowFlag[index]>>indexBit)&0x01 == OLED_TRUE){
        return indexCount;
      }
      else{
        indexCount++;
      }
    }
  }
  return indexCount;
}

void OLED_GUIWindowTitleSet(unsigned char windowNumber,unsigned char * setTitle){
  OLED_GUIWindowTitleAddress[windowNumber] = setTitle;
}

unsigned char * OLED_GUIWindowTitleGet(unsigned char windowNumber){
  return OLED_GUIWindowTitleAddress[windowNumber];
}

void OLED_GUIWindowAdd(unsigned char windowNumber,unsigned char * setTitle,OLED_SYS_BIT_COUNT setWindowInitAddress,unsigned char * setWindowPlaneAddress,OLED_SYS_BIT_COUNT setKeyHandleAddress){
  OLED_GUIEnableWindowFlagSet(windowNumber,OLED_TRUE);
  OLED_GUIWindowTitleSet(windowNumber,setTitle);
  OLED_GUIWindowInitAddress[windowNumber] = setWindowInitAddress;
  OLED_GUIWindowPlaneAddress[windowNumber] = setWindowPlaneAddress;
  OLED_GUIPlaneClear(windowNumber);
  OLED_GUIKeyHandleAddress[windowNumber] = setKeyHandleAddress;
}

void OLED_GUIInit(unsigned char * setHomeWindowTitle,OLED_SYS_BIT_COUNT setWindowInitAddress,unsigned char * setWindowPlaneAddress){
  unsigned index;
  for(index=0;index<OLED_GUI_WINDOW_NUMBER_MAX;index++){
    OLED_GUIEnableWindowFlagSet(index,OLED_FALSE);
  }
  OLED_GUIWindowAdd(OLED_GUI_HOME_WINDOW_NUMBER,setHomeWindowTitle,(OLED_SYS_BIT_COUNT)setWindowInitAddress,setWindowPlaneAddress,(OLED_SYS_BIT_COUNT)OLED_GUIKeyHandleHome);
  OLED_GUIFocusWindowFlagSet(OLED_GUI_HOME_WINDOW_NUMBER);
}

void OLED_GUIFlash(void){
  unsigned char x,y;
  OLED_BufferClear();
  OLED_DrawLine(0,OLED_HEIGHT-OLED_GUI_TITLE_CHAR_HEIGHT-1,OLED_DRAW_WIDTH_MAX,OLED_HEIGHT-OLED_GUI_TITLE_CHAR_HEIGHT-1,OLED_COLOR_WHITE);
  //画Title分割线
  OLED_DrawString8X16(0,OLED_HEIGHT-OLED_GUI_TITLE_CHAR_HEIGHT,OLED_GUIWindowTitleGet(OLED_GUIFocusWindowFlagGet()),OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  //画Title
  ((void(*)(unsigned char))OLED_GUIWindowInitAddress[OLED_GUIFocusWindowFlagGet()])(OLED_GUIFocusWindowFlagGet());
  //执行窗口获取焦点函数
  for(x=0;x<=OLED_GUI_PLANE_MAX_X;x++){
    for(y=0;y<=OLED_GUI_PLANE_MAX_Y;y++){
      OLED_DrawPoint(x,y,OLED_GUIPlanePointGet(OLED_GUIFocusWindowFlagGet(),x,y));
    }
  }
  //画Plane
}

void OLED_GUIPlanePointSet(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char setBit){
  if(setBit){
    OLED_GUIWindowPlaneAddress[windowNumber][(y*OLED_WIDTH+x)/8] |= (0x01<<((y*OLED_WIDTH+x)%8));
  }
  else{
    OLED_GUIWindowPlaneAddress[windowNumber][(y*OLED_WIDTH+x)/8] &= ~(0x01<<((y*OLED_WIDTH+x)%8));
  }
}

unsigned char OLED_GUIPlanePointGet(unsigned char windowNumber,unsigned char x,unsigned char y){
  return (OLED_GUIWindowPlaneAddress[windowNumber][(y*OLED_WIDTH+x)/8]>>((y*OLED_WIDTH+x)%8))&0x01;
}

void OLED_GUIPlaneClear(unsigned char windowNumber){
  unsigned x,y;
  for(x=0;x<=OLED_GUI_PLANE_MAX_X;x++){
    for(y=0;y<=OLED_GUI_PLANE_MAX_Y;y++){
      OLED_GUIPlanePointSet(windowNumber,x,y,OLED_COLOR_BLACK);
    }
  }
}

void OLED_GUIPlaneFill(unsigned char windowNumber){
  unsigned x,y;
  for(x=0;x<=OLED_GUI_PLANE_MAX_X;x++){
    for(y=0;y<=OLED_GUI_PLANE_MAX_Y;y++){
      OLED_GUIPlanePointSet(windowNumber,x,y,OLED_COLOR_WHITE);
    }
  }
}

void OLED_GUIPlaneDrawPoint(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char color){
  OLED_GUIPlanePointSet(windowNumber,x,y,color);
}

void OLED_GUIPlaneDrawLine(unsigned char windowNumber,unsigned char startX,unsigned char startY,unsigned char endX,unsigned char endY,unsigned char color){
  unsigned char x,y,temp;
  if(startX>endX){
    temp = startX;
    startX = endX;
    endX = temp;
    temp = startY;
    startY = endY;
    endY = temp;
  }
  if(startX == endX){
    if(startY>endY){
      temp = startY;
      startY = endY;
      endY = temp;
    }
    for(y=startY;y<=endY;y++){
      OLED_GUIPlaneDrawPoint(windowNumber,startX,y,color);
    }
  }
  else{
    for(x=startX;x<=endX;x++){
      y = (x-startX)*(endY-startY)/(endX-startX)+startY;
      OLED_GUIPlaneDrawPoint(windowNumber,x,y,color);
    }
  }
}

void OLED_GUIPlaneDrawCircle(unsigned char windowNumber,unsigned char centerX,unsigned char centerY,unsigned char radius,unsigned char color,unsigned char fillBool){
  unsigned char x,y;
  for(x=0;x<radius;x++){
    y = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(x*x)));
    if(centerY-y<0){
      y=centerY;
      //预防浮点计算
    }
    OLED_GUIPlaneDrawPoint(windowNumber,centerX+x,centerY+y,color);
    //右上
    OLED_GUIPlaneDrawPoint(windowNumber,centerX-x,centerY+y,color);
    //左上
    OLED_GUIPlaneDrawPoint(windowNumber,centerX+x,centerY-y,color);
    //右下
    OLED_GUIPlaneDrawPoint(windowNumber,centerX-x,centerY-y,color);
    //左下
    
    if(fillBool == OLED_TRUE){
      OLED_GUIPlaneDrawLine(windowNumber,centerX+x,centerY-y,centerX+x,centerY+y,color);
      //右上下
      OLED_GUIPlaneDrawLine(windowNumber,centerX-x,centerY-y,centerX-x,centerY+y,color);
      //左上下
    }
  }
  for(y=0;y<radius;y++){
    x = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(y*y)));
    OLED_GUIPlaneDrawPoint(windowNumber,centerX+x,centerY+y,color);
    //右上
    OLED_GUIPlaneDrawPoint(windowNumber,centerX-x,centerY+y,color);
    //左上
    OLED_GUIPlaneDrawPoint(windowNumber,centerX+x,centerY-y,color);
    //右下
    OLED_GUIPlaneDrawPoint(windowNumber,centerX-x,centerY-y,color);
    //左下
  }
}

void OLED_GUIPlaneDrawRectangle(unsigned char windowNumber,unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char color,unsigned char fillBool){
  OLED_GUIPlaneDrawLine(windowNumber,startX,startY,startX+width-1,startY,color);
  //底边
  OLED_GUIPlaneDrawLine(windowNumber,startX,startY+height-1,startX,startY,color);
  //左边
  OLED_GUIPlaneDrawLine(windowNumber,startX,startY+height-1,startX+width-1,startY+height-1,color);
  //顶边
  OLED_GUIPlaneDrawLine(windowNumber,startX+width-1,startY+height-1,startX+width-1,startY,color);
  //右边
  
  if(fillBool == OLED_TRUE){
    unsigned char x;
    for(x=startX;x<startX+width;x++){
      OLED_GUIPlaneDrawLine(windowNumber,x,startY,x,startY+height-1,color);
    }
  }
}

void OLED_GUIPlaneDrawTriangle(unsigned char windowNumber,unsigned char upX,unsigned char upY,unsigned char leftX,unsigned char leftY,unsigned char rightX,unsigned char rightY,unsigned char color){
  OLED_GUIPlaneDrawLine(windowNumber,leftX,leftY,rightX,rightY,color);
  //底边
  OLED_GUIPlaneDrawLine(windowNumber,leftX,leftY,upX,upY,color);
  //左边
  OLED_GUIPlaneDrawLine(windowNumber,rightX,rightY,upX,upY,color);
  //右边
}

void OLED_GUIPlaneDrawRoundedRectangle(unsigned char windowNumber,unsigned char startX,unsigned char startY,unsigned width,unsigned height,unsigned char radius,unsigned char color){
  unsigned char x,y;
  OLED_GUIPlaneDrawLine(windowNumber,startX+radius,startY,startX+width-1-radius,startY,color);
  //底边
  OLED_GUIPlaneDrawLine(windowNumber,startX+radius,startY+height-1,startX+width-1-radius,startY+height-1,color);
  //顶边
  OLED_GUIPlaneDrawLine(windowNumber,startX,startY+height-1-radius,startX,startY+radius,color);
  //左边
  OLED_GUIPlaneDrawLine(windowNumber,startX+width-1,startY+height-1-radius,startX+width-1,startY+radius,color);
  //右边
 
  for(x=0;x<radius;x++){
    y = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(x*x)));
    OLED_GUIPlaneDrawPoint(windowNumber,startX+width-1-radius+x,startY+height-1-radius+y,color);
    //右上
    OLED_GUIPlaneDrawPoint(windowNumber,startX+radius-x,startY+height-1-radius+y,color);
    //左上
    OLED_GUIPlaneDrawPoint(windowNumber,startX+width-1-radius+x,startY+radius-y,color);
    //右下
    OLED_GUIPlaneDrawPoint(windowNumber,startX+radius-x,startY+radius-y,color);
    //左下
  }
  for(y=0;y<radius;y++){
    x = (unsigned char)(OLED_DRAW_SQRT((radius*radius)-(y*y)));
    OLED_GUIPlaneDrawPoint(windowNumber,startX+width-1-radius+x,startY+height-1-radius+y,color);
    //右上
    OLED_GUIPlaneDrawPoint(windowNumber,startX+radius-x,startY+height-1-radius+y,color);
    //左上
    OLED_GUIPlaneDrawPoint(windowNumber,startX+width-1-radius+x,startY+radius-y,color);
    //右下
    OLED_GUIPlaneDrawPoint(windowNumber,startX+radius-x,startY+radius-y,color);
    //左下
  }
}

void OLED_GUIPlaneDrawChar8X16(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char GUIPlaneDrawChar,unsigned char color,unsigned char fillBool,unsigned char angle){
  unsigned char xPos,yPos;
  if(angle == OLED_ANGLE_0){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[GUIPlaneDrawChar-' '][15-yPos]>>xPos)&0x01 != 0){
          OLED_GUIPlaneDrawPoint(windowNumber,x+xPos,y+yPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_GUIPlaneDrawPoint(windowNumber,x+xPos,y+yPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_GUIPlaneDrawPoint(windowNumber,x+xPos,y+yPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
  else if(angle == OLED_ANGLE_90){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[GUIPlaneDrawChar-' '][yPos]>>xPos)&0x01 != 0){
          OLED_GUIPlaneDrawPoint(windowNumber,x+yPos,y+xPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_GUIPlaneDrawPoint(windowNumber,x+yPos,y+xPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_GUIPlaneDrawPoint(windowNumber,x+yPos,y+xPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
  else if(angle == OLED_ANGLE_180){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[GUIPlaneDrawChar-' '][yPos]>>xPos)&0x01 != 0){
          OLED_GUIPlaneDrawPoint(windowNumber,x+7-xPos,y+yPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_GUIPlaneDrawPoint(windowNumber,x+7-xPos,y+yPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_GUIPlaneDrawPoint(windowNumber,x+7-xPos,y+yPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
  else if(angle == OLED_ANGLE_270){
    for(yPos=0;yPos<16;yPos++){
      for(xPos=0;xPos<8;xPos++){
        if((F8X16[GUIPlaneDrawChar-' '][15-yPos]>>(7-xPos))&0x01 != 0){
          OLED_GUIPlaneDrawPoint(windowNumber,x+yPos,y+xPos,color);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_WHITE){
          OLED_GUIPlaneDrawPoint(windowNumber,x+yPos,y+xPos,OLED_COLOR_BLACK);
        }
        else if(fillBool == OLED_TRUE && color == OLED_COLOR_BLACK){
          OLED_GUIPlaneDrawPoint(windowNumber,x+yPos,y+xPos,OLED_COLOR_WHITE);
        }
      }
    }
  }
}

void OLED_GUIPlaneDrawString8X16(unsigned char windowNumber,unsigned char x,unsigned char y,unsigned char * GUIPlaneDrawString,unsigned char color,unsigned char fillBool,unsigned char angle,unsigned uprightBool){
  while(*GUIPlaneDrawString != '\0'){
    OLED_GUIPlaneDrawChar8X16(windowNumber,x,y,*GUIPlaneDrawString,color,fillBool,angle);
    if(uprightBool == OLED_FALSE){
      if(angle == OLED_ANGLE_0){
        x+=8;
      }
      else if(angle == OLED_ANGLE_90){
        y+=8;
      }
      else if(angle == OLED_ANGLE_180){
        x-=8;
      }
      else if(angle == OLED_ANGLE_270){
        y-=8;
      }
    }
    else{
      if(angle == OLED_ANGLE_0){
        y-=16;
      }
      else if(angle == OLED_ANGLE_90){
        x+=16;
      }
      else if(angle == OLED_ANGLE_180){
        y+=16;
      }
      else if(angle == OLED_ANGLE_270){
        x-=16;
      }
    }
    GUIPlaneDrawString++;
  }
}

void OLED_GUIKeyInput(unsigned char inputKey){
  if(inputKey == OLED_GUI_KEY_HOME_CLICK){
    OLED_GUIFocusWindowFlagSet(OLED_GUI_HOME_WINDOW_NUMBER);
    //长按进入HOME
    ((void(*)(unsigned char,unsigned char))OLED_GUIKeyHandleAddress[OLED_GUI_HOME_WINDOW_NUMBER])(OLED_GUI_HOME_WINDOW_NUMBER,inputKey);
    //进入HOME处理函数
  }
  else{
    ((void(*)(unsigned char,unsigned char))OLED_GUIKeyHandleAddress[OLED_GUIFocusWindowFlagGet()])(OLED_GUIFocusWindowFlagGet(),inputKey);
  }
}

void OLED_GUIKeyHandleHome(unsigned char windowNumber,unsigned char inputKey){
  static unsigned char indexSelectWindow = 1;
  unsigned char indexBehindWindow,indexAboveWindow;
  switch(inputKey){
  case OLED_GUI_KEY_UP_CLICK:
    if(indexSelectWindow-1 <= 0){
      indexSelectWindow = OLED_GUI_WINDOW_NUMBER_MAX-1;
    }
    else{
      indexSelectWindow--;
    }
    break;
  case OLED_GUI_KEY_LEFT_CLICK:
    //gpio_turn(G1);
    break;
  case OLED_GUI_KEY_CENTRE_CLICK:
    OLED_GUIFocusWindowFlagSet(indexSelectWindow);
    break;
  case OLED_GUI_KEY_HOME_CLICK:
    break;
  case OLED_GUI_KEY_RIGHT_CLICK:
    break;
  case OLED_GUI_KEY_DOWN_CLICK:
    if(indexSelectWindow+1 >= OLED_GUI_WINDOW_NUMBER_MAX){
      indexSelectWindow = 1;
    }
    else{
      indexSelectWindow++;
    }
    break;
  default:
    break;
  }
  //循环寻找开启窗口
  while(OLED_GUIEnableWindowFlagGet(indexSelectWindow) == OLED_FALSE){
    indexSelectWindow++;
    if(indexSelectWindow >= OLED_GUI_WINDOW_NUMBER_MAX){
      indexSelectWindow = 1;
    }
  }
  //寻找下一个开启窗口
  if(indexSelectWindow+1 >= OLED_GUI_WINDOW_NUMBER_MAX){
    indexBehindWindow = 1;
  }
  else{
    indexBehindWindow = indexSelectWindow+1;
  }
  while(OLED_GUIEnableWindowFlagGet(indexBehindWindow) == OLED_FALSE){
    if(indexBehindWindow+1 >= OLED_GUI_WINDOW_NUMBER_MAX){
      indexBehindWindow = 1;
    }
    else{
      indexBehindWindow++;
    }
    if(indexBehindWindow == indexSelectWindow){
      break;
      //一个循环
    }
  }
  //寻找上一个开启窗口
  if(indexSelectWindow-1 <= 0){
    indexAboveWindow = OLED_GUI_WINDOW_NUMBER_MAX-1;
  }
  else{
    indexAboveWindow = indexSelectWindow-1;
  }
  while(OLED_GUIEnableWindowFlagGet(indexAboveWindow) == OLED_FALSE){
    if(indexAboveWindow-1 <= 0){
      indexAboveWindow = OLED_GUI_WINDOW_NUMBER_MAX-1;
    }
    else{
      indexAboveWindow--;
    }
    if(indexAboveWindow == indexSelectWindow){
      break;
      //一个循环
    }
  }
  //画Home Plane
  OLED_GUIPlaneClear(windowNumber);
  OLED_GUIPlaneDrawString8X16(windowNumber,OLED_GUI_PLANE_MAX_X-(OLED_GUI_TITLE_CHAR_WIDTH*2),OLED_GUI_TITLE_CHAR_HEIGHT,"<-",OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  OLED_GUIPlaneDrawString8X16(windowNumber,0,OLED_GUI_TITLE_CHAR_HEIGHT,OLED_GUIWindowTitleGet(indexSelectWindow),OLED_COLOR_BLACK,OLED_TRUE,OLED_ANGLE_0,OLED_FALSE);
  if(indexBehindWindow != indexSelectWindow){
    OLED_GUIPlaneDrawString8X16(windowNumber,0,0,OLED_GUIWindowTitleGet(indexBehindWindow),OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  }
  if(indexAboveWindow != indexSelectWindow && indexAboveWindow != indexBehindWindow){
    OLED_GUIPlaneDrawString8X16(windowNumber,0,OLED_GUI_TITLE_CHAR_HEIGHT*2,OLED_GUIWindowTitleGet(indexAboveWindow),OLED_COLOR_WHITE,OLED_FALSE,OLED_ANGLE_0,OLED_FALSE);
  }
}
