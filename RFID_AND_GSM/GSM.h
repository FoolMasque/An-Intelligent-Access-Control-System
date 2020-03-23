/************************************************************
	
	程序说明：
	功能：TEXT(GSM)模式下发送英文短消息，短消息直接显示在终端
	首先要确定模块已经注册到网络
	然后正确的硬件连接   P3.0-----STXD或者5VT   P3.1-----SRXD或者5VR   GND---GND（只要保证公地即可，没必要单独接一次）
	然后确认你单片机上的晶振，根据晶振修改自己的程序。
	推荐先将单片机与电脑相连，确定单片机发送的数据是正确的。如果发送的是乱码，请检查晶振与单片机的串口波特率。
	如果通过以上几条还解决不了问题，请看群共享文件 AN0004 。

*************************************************************/
#include <REG52.H>
#include <string.H>
#include <intrins.h>
#define u8 unsigned char
#define u16 unsigned int
//以下是板子上LED的配置，把Px_x改成自己对应的脚。
//以下是你的51单片机的晶振大小
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^0;
sbit LCD1602_RW=P2^2;
sbit LCD1602_RS=P2^1;

/*以上是液晶宏定义*/

//#define FOSC_110592M
//#define FOSC_12M
void Lcd1602_Delay1ms(u8 c);
void LcdWriteCom(u8 com);
void LcdWriteData(u8 dat);
void LcdInit();
void SerialInti();
void Uart1Send(u8 c);
void Uart1Sends(u8 *str);
void DelaySec(int sec);
