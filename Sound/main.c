#include <ioCC2530.h>
#include <string.h>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1000; j++) {
            // 延时约1ms
        }
    }
}

void SYN_FrameInfo(unsigned char Music,unsigned char *HZdata);
void Uart1_Init();
void Uart1_Send_String(unsigned char *Data, unsigned char len);

void main(void) {
	Uart1_Init();

    while (1) {
		SYN_FrameInfo(0, "乐声电子科技");
		delay_ms(10000);
	}
}

void Uart1_Init()
{
	CLKCONCMD &= ~0x40;
	while(CLKCONSTA & 0x40);
	CLKCONCMD &= ~0x47;

	PERCFG &= ~(1 << 0); //位置1 P0口
	P0SEL |= 0x30; //P0.2和P0.3设置为外设功能
	P2DIR &= ~0xc0;

	U1CSR |= 0x80;
	U1GCR &= ~0x1F;
	U1GCR |= 8;
	U1BAUD = 59; //波特率设置为9600
	UTX1IF = 1; //发送中断标志位
	U1CSR |= 0x40; //允许接收
	IEN0 |= 0x84; //开总中断和串口0接收中断
}

void Uart1_Send_String(unsigned char *Data, unsigned char len)
{
	unsigned char i;
	for(i = 0; i < len; i++)
	{
		U1DBUF = Data[i];
		
		while(UTX1IF == 0);
		UTX1IF = 0;
	}
}
/***********************************************************
* 名    称：  YS-SYN6288 文本合成函数
* 功    能：  发送合成文本到SYN6288芯片进行合成播放
* 入口参数：Music(背景音乐选择):0无背景音乐。1-15：相关背景音乐
            *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（0，“乐声电子科技”）；
**********************************************************/
void SYN_FrameInfo(unsigned char Music,unsigned char *HZdata)
{
/****************需要发送的文本**********************************/ 
		 unsigned  char  Frame_Info[200];
         unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  			//定义校验字节
	     unsigned  int i=0; 
		 HZ_Length =strlen(HZdata); 			//需要发送文本的长度
 
/*****************帧固定配置信息**************************************/           
		 Frame_Info[0] = 0xFD ; 			//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
		 Frame_Info[4] = 0x01 | Music<<4 ;  //构造命令参数：背景音乐设定

/*******************校验码计算***************************************/		 
		 for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	     {  
	         ecc=ecc^(Frame_Info[i]);		//对发送的字节进行异或校验	
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//依次发送待合成的文本数据
	     {  
	         ecc=ecc^(HZdata[i]); 				//对发送的字节进行异或校验		
	     }		 
/*******************发送帧信息***************************************/		  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  Uart1_Send_String(Frame_Info,5+HZ_Length+1);
}
