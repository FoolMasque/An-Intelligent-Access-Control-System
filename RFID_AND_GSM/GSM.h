/************************************************************
	
	����˵����
	���ܣ�TEXT(GSM)ģʽ�·���Ӣ�Ķ���Ϣ������Ϣֱ����ʾ���ն�
	����Ҫȷ��ģ���Ѿ�ע�ᵽ����
	Ȼ����ȷ��Ӳ������   P3.0-----STXD����5VT   P3.1-----SRXD����5VR   GND---GND��ֻҪ��֤���ؼ��ɣ�û��Ҫ������һ�Σ�
	Ȼ��ȷ���㵥Ƭ���ϵľ��񣬸��ݾ����޸��Լ��ĳ���
	�Ƽ��Ƚ���Ƭ�������������ȷ����Ƭ�����͵���������ȷ�ġ�������͵������룬���龧���뵥Ƭ���Ĵ��ڲ����ʡ�
	���ͨ�����ϼ���������������⣬�뿴Ⱥ�����ļ� AN0004 ��

*************************************************************/
#include <REG52.H>
#include <string.H>
#include <intrins.h>
#define u8 unsigned char
#define u16 unsigned int
//�����ǰ�����LED�����ã���Px_x�ĳ��Լ���Ӧ�Ľš�
//���������51��Ƭ���ľ����С
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^0;
sbit LCD1602_RW=P2^2;
sbit LCD1602_RS=P2^1;

/*������Һ���궨��*/

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
