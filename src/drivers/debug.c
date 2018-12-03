#include"header.h"
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++)
    {
        CRC_Temp ^= Buf[i];//按位异或
        for (j=0;j<8;j++)
         {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        } 
     }
    return(CRC_Temp);
}

void OutPut_Data(unsigned short int* OutData)
{
    unsigned short ChxData[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {
        ChxData[i] = (unsigned short)*(OutData+i);
    }
    for(i=0;i<4;i++)
    {
        databuf[i*2+0] = (unsigned char)((ChxData[i])&0xff);
        databuf[i*2+1] = (unsigned char)((ChxData[i])>>8);
    }
    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16&0xff;
    databuf[9] = CRC16>>8;
    for(i=0;i<10;i++)
      Uart_SendChar (UARTR2, databuf[i]);//根据串口修改
}
void GetData(float Ch1,float Ch2,float Ch3,float Ch4,unsigned short int* OutData)
{
  int temp[4];
  temp[0]=(int)Ch1;
  temp[1]=(int)Ch2;
  temp[2]=(int)Ch3;
  temp[3]=(int)Ch4;
 // temp[4]=(int)Ch5;
 // temp[5]=(int)Ch6;
  *OutData=(unsigned int)temp[0];
  *(OutData+1)=(unsigned int)temp[1];
  *(OutData+2)=(unsigned int)temp[2];
  *(OutData+3)=(unsigned int)temp[3];
  //*(OutData+4)=(unsigned int)temp[4];
 // *(OutData+5)=(unsigned int)temp[5];
}

