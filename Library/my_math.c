/**************************************************************************
  * @brief     : 数学函数库
  * @author    : 陈梦洋(mengyang.chen@leadchina.cn)
  * @copyright : LeadChina
  * @version   : V1.0
  * @note      : 注意事项
  * @history   : 历史记录
***************************************************************************/
#include "my_math.h"

float MaxMinLimit(float input,float min,float max)
{
    if(input > max) input = max;
    if(input < min) input = min;
    return input;
}

float Calc_Deadband(float dat,float deadband)
{
	if(dat > deadband) dat -= deadband;
	else if(dat < -deadband) dat += deadband;
	else dat = 0;
	return dat;
}

void LSM_Plus(float X,float Y,float* SUMX,float* SUMX2,float* SUMY,float* SUMXY,float* SUMY2)
{
	*SUMX  += X;
	*SUMX2 += (X*X);
	*SUMY  += Y;
	*SUMXY += (X*Y);
	*SUMY2 += (Y*Y);
}

/*******************************************************************************
* 函数名 : 
* 作  者 : 陈梦洋
* 描  述 : 
* 输  入 : 无
* 输  出 : 无
* 返回值 : 无
* 说  明 :
*******************************************************************************/
void LSM_Output(float N,float SUMX,float SUMX2,float SUMY,float SUMXY,float SUMY2,float* K,float* B,float* R )
{

	*K =(N*SUMXY-SUMX*SUMY)/(N*SUMX2-SUMX*SUMX);
	*B =(SUMX2*SUMY-SUMX*SUMXY)/(N*SUMX2-SUMX*SUMX);
	*R =(SUMXY/N-SUMY/N*SUMX/N)/sqrt((SUMX2/N-SUMX/N*SUMX/N)*(SUMY2/N-SUMY/N*SUMY/N));
}

float RCLowPass(float newDat,float oldDat,float f0,float Ts)
{
	float a;
	if(Ts==0)
	{
		return oldDat;
	}
	a = f0/(1/Ts+f0);
	LowPassSimple(newDat,oldDat,a);
	return oldDat;
}

uint16_t CalculateCheckSumScheduler(uint8_t *p,int len)
{
    uint16_t checksum = 0;

    for(uint16_t i = 0;i<len;i+=2)
    {
        checksum+=(p[i]<<8 | p[i+1]);
    }
    return checksum;
}

uint16_t CalculateCheckSumWord(uint8_t *p,int len)
{
    uint16_t checksum = 0;

    for(uint16_t i = 0;i<len;i++)
    {
        checksum+=p[i];
    }
    return checksum;
}

uint8_t CalculateCheckSumByte(uint8_t *p,int len)
{
    uint8_t checksum = 0;

    for(uint16_t i = 0;i<len;i++)
    {
        checksum+=p[i];
    }
    return checksum;
}
void EncodeAscii(uint8_t* input,uint8_t* output,uint8_t length)
{
    for(uint8_t i=0;i<length;i++)
    {
        if(((*(input+i)) >> 4) < 0x0A)
            *(output+2*i) = ((*(input+i)) >> 4)+'0';
        else 
            *(output+2*i) = ((*(input+i)) >> 4)+'A'-10;
        if(((*(input+i)) & 0x0F) < 0x0A)
            *(output+2*i+1) = ((*(input+i)) & 0x0F)+'0';
        else 
            *(output+2*i+1) = ((*(input+i)) & 0x0F)+'A'-10;
    }
}

void DecodeAscii(uint8_t* input,uint8_t* output,uint8_t length)
{
    for(uint8_t i=0;i<length;i++)
    {
        if((*(input+2*i)) < 'A')
            *(output+i) = (*(input+2*i)) - '0';
        else 
            *(output+i) = (*(input+2*i)) - 'A'+10;
        *(output+i) <<= 4;
        if((*(input+2*i+1)) < 'A')
            *(output+i) |= (*(input+2*i+1)) - '0';
        else 
            *(output+i) |= (*(input+2*i+1)) - 'A'+10;
    }
}

void BigToLittle(uint8_t* ptr,uint8_t len)
{
    uint8_t _temp[8];
    for(uint8_t i=0;i<len;i++)
        _temp[i] = ptr[i];
    for(uint8_t i=0;i<len;i++)
    {
        ptr[len-1-i] = _temp[i];
    }
}
    
DECLARE_SimpleRCLowPassFilter(1)
DECLARE_SimpleRCLowPassFilter(2)
DECLARE_SimpleRCLowPassFilter(10)
DECLARE_SimpleRCLowPassFilter(100)
DECLARE_SimpleRCLowPassFilter(200)

