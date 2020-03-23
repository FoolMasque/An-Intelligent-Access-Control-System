//////////////////////////////////
//端口定义
/////////////////////////////////////////////////////////////////////
//MFRC500
sbit     MF522_RST  =    P2^3;                   //RC500片选
sbit     MF522_NSS  =    P2^7;                   //SDA
sbit     MF522_SCK  =    P2^6;
sbit     MF522_SI   =    P2^5;
sbit     MF522_SO   =    P2^4;
void Delay(unsigned int time); 

