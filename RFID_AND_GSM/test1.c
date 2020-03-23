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
#include <GSM.h>
#include <string.h>
#include <main.h>
#include <MFRC522.h>
#define u8 unsigned char
#define u16 unsigned int

sbit MOTOR_DRIVER0 = P1^0;
sbit MOTOR_DRIVER1 = P1^1;
sbit SPK = P1^2;
unsigned int  rec_data_len_uart=0;    //标记Buffer_Uart0接收数组
unsigned char xdata Buffer_Uart0_Rec[100]={0};		 //Uart0中断接收数组											
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char g_ucTempbuf[20];
unsigned char ID_DATA[]="LMJY-";  //ID地址 

u8 xdata system_ok[]="SYSTEM OK!";
u8 xdata open_MSG[]="The door is opened";
u8 xdata close_MSG[]="Door closed!";
u8 xdata open_MSG1[]=":Come in!";
void clear_rec_data()
{
	u8 i,temp_len;
	temp_len=strlen(Buffer_Uart0_Rec);
	if(temp_len>100)
	{
		temp_len=100;
	}
	for(i=0;i<temp_len;i++)
	{
		Buffer_Uart0_Rec[i]='\0';	
	}
	rec_data_len_uart=0;
}

/*串行通讯中断，收发完成将进入该中断*/
void Serial_interrupt() interrupt 4 
{
	unsigned char temp_rec_data_uart0;	
	temp_rec_data_uart0 = SBUF;//读取接收数据		
	RI=0;//接收中断信号清零，表示将继续接收			
	Buffer_Uart0_Rec[rec_data_len_uart]=temp_rec_data_uart0;	//接收数据
	rec_data_len_uart++;
	if(rec_data_len_uart>100)
	{
		rec_data_len_uart=0; 	//从头开始接收数据
	}	
} 
void System_Init()
{
 u8 i=0;
 DelaySec(5);
 Uart1Sends("ATE0\r\n");
 DelaySec(1);
 Uart1Sends("AT+CMGF=1\r\n");
 DelaySec(1);
 Uart1Sends("AT+CSCS=\"GSM\"\r\n");
 DelaySec(1);
 Uart1Sends("AT+CNMI=2,2,0,0,0\r\n");
 DelaySec(1);
 clear_rec_data();
 LcdWriteCom(0x83);
 while(system_ok[i]!='\0')
 {
  LcdWriteData(system_ok[i]);
  Lcd1602_Delay1ms(5);
  i++;
 }
}

void Delay(unsigned int time)//延时函数
{
  unsigned int i,k  ;
  for(i=0;i<255;i++)
    for(k=0;k<time;k++)
      _nop_();	  
}

void main()
{
	u8 x = 0;
	unsigned char status;
	LcdInit();
	SerialInti();
	System_Init();
	PcdReset();
    PcdAntennaOff(); //关闭天线
    PcdAntennaOn();  //开启天线
	while(1)
	{
	 status = PcdRequest(PICC_REQALL, g_ucTempbuf); //寻卡
     if (status == MI_OK)//有卡
     { 	 			 
		         //此处可加蜂鸣器程序
	  // TYPE_DATA[0]=g_ucTempbuf[0];
	  // TYPE_DATA[1]=g_ucTempbuf[1];
	   status = PcdAnticoll(g_ucTempbuf);       //防冲撞 ，返回卡片ID号 4字节
	   if (status == MI_OK)
	   { 
		 status = PcdSelect(g_ucTempbuf);    //选定卡片  ,输入卡片ID号
		 if (status == MI_OK)
		 { 
		  //ID_DATA[0]=g_ucTempbuf[0];
		 // ID_DATA[1]=g_ucTempbuf[1];
		 // ID_DATA[2]=g_ucTempbuf[2];
		 // ID_DATA[3]=g_ucTempbuf[3];

		  status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//PICC_AUTHENT1A 验证密码模式60A密码 61B密码，“1”块地址，DefaultKey初始密码，g_ucTempbuf ID号
	      if (status == MI_OK) //验证密码
	      {	 																																														  
            status = PcdRead(1, g_ucTempbuf);//读块地址“1”的数据，返回值存在	g_ucTempbuf
		    if (status == MI_OK)
			{ 
		     //value_DATA[0]=g_ucTempbuf[0];
			 //value_DATA[1]=g_ucTempbuf[1];
			 //value_DATA[2]=g_ucTempbuf[2];
			 //value_DATA[3]=g_ucTempbuf[3];	
             //			 b=value_DATA[3];
			 
			 if(strstr(g_ucTempbuf+8,"00000000")!=NULL)
			 {
			  x=(g_ucTempbuf[4]-0x30)*100+(g_ucTempbuf[5]-0x30)*10+(g_ucTempbuf[6]-0x30);
			  if(x>=100)
			  {
			   ID_DATA[5]=g_ucTempbuf[4];
			   ID_DATA[6]=g_ucTempbuf[5];
			   ID_DATA[7]=g_ucTempbuf[6];
			  }
			  if(x>=10&&x<100)
			  {
			   ID_DATA[5]=g_ucTempbuf[5];
			   ID_DATA[6]=g_ucTempbuf[6];
			  }
			  if(x<10)
			  {
			   ID_DATA[5]=g_ucTempbuf[6];
			   x=0;
			   while(ID_DATA[x]!='\0')
			   {
			   	if(x>5)
				ID_DATA[x]='\0';
				x++;
			   }
			  } 
			  x=0;
			  LcdWriteCom(0x80+0x40);
              while(ID_DATA[x]!='\0')
              {
               LcdWriteData(ID_DATA[x]);
               Lcd1602_Delay1ms(5);
               x++;
              }
			  x=0;
			  MOTOR_DRIVER0 = 1;
		      MOTOR_DRIVER1 = 0;
		      DelaySec(2);
	 	      MOTOR_DRIVER0 = 1;
		      MOTOR_DRIVER1 = 1;
			  while(open_MSG1[x]!='\0')
			  {
			   LcdWriteData(open_MSG1[x]);
               Lcd1602_Delay1ms(5);
               x++;
			  }
			  x=0;
			  Uart1Sends("AT+CMGS=\"15108911893\"\r\n");//此处修改为对方的电话号
              DelaySec(1);//延时3秒
              Uart1Sends(ID_DATA);//修改短信内容,短信内容可为英文和数字
              Uart1Send(0x1a);
			  clear_rec_data();
			  DelaySec(4);
			  LcdWriteCom(0x01);
			  LcdWriteCom(0x83);
              while(system_ok[x]!='\0')
              {
               LcdWriteData(system_ok[x]);
               Lcd1602_Delay1ms(5);
               x++;
              }
			  x=0;
			 }																     
			}   
          }
		 } 	  
		}
	 }  
	 if(strstr(Buffer_Uart0_Rec,"15108911893")!=NULL)
	 {
		DelaySec(1);
		//Uart1Sends("Received");
		//Lcd1602_Delay1ms(10);
	//	Uart1Sends(Buffer_Uart0_Rec);
	    MOTOR_DRIVER0 = 1;
		MOTOR_DRIVER1 = 0;
		DelaySec(5);
		clear_rec_data();
		MOTOR_DRIVER0 = 1;
		MOTOR_DRIVER1 = 1;
		LcdWriteCom(0x80+0x40);
        while(open_MSG[x]!='\0')
        {
         LcdWriteData(open_MSG[x]);
         Lcd1602_Delay1ms(5);
         x++;
        }
		x=0;
		DelaySec(2);
		LcdWriteCom(0x01);
		LcdWriteCom(0x80+0x42);
        while(close_MSG[x]!='\0')
        {
         LcdWriteData(close_MSG[x]);
         Lcd1602_Delay1ms(5);
         x++;
        }
		x=0;
	//	Uart1Sends("ok");
	 }						  
	 //Lcd1602_Delay1ms(50);
	}


}
