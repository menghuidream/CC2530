/***************************************
* copyright: jing
* filename: main.c
* description: Serial Port Communication
* author: jing
* phone: ***********
* version: 1.0
* date: 2023-7-5
***************************************/

#include <ioCC2530.h>
#include <string.h>
#include <stdio.h>

#define uint unsigned int
#define uchar unsigned char

uchar rxTemp = 0;

void Beep_OFF(); //蜂鸣器关闭函数声明
void Delay_Ms(uint ms); //延时函数声明
void Uart0_Init(); //串口初始化函数声明
void Uart0_Send_String(uchar *Data, uint len); //串口发送字符串函数声明

void main()
{
    Beep_OFF();
    Uart0_Init();
    while(1)
    {
	  	//printf("%d",rxTemp);
        if(rxTemp != 0&&rxTemp == 50)
        {
            Uart0_Send_String("2748561", 11);
			
            rxTemp = 0;
            Delay_Ms(50000);
        }
		else
		{
        Uart0_Send_String("2846750", 11);
        Delay_Ms(50000);
		}
    }
}
/***************************************
* description: 蜂鸣器关闭函数
* parameter: none
* return: none
* date: 2023-7-5
***************************************/
void Beep_OFF()
{
    P1DIR |= 0x80; //P1.7设置为输出
    P1SEL &= ~0x80; //P1.7设置为通用I/O
    P1_7 = 0; //P1.7输出低电平
}
/***************************************
* description: 延时函数
* parameter: ms
* return: none
* date: 2023-7-5
***************************************/
void Delay_Ms(uint ms)
{
    uint i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 120; j++);
    }
}
/***************************************
* description: 串口初始化函数
* parameter: none
* return: none
* date: 2023-7-5
***************************************/
void Uart0_Init()
{
    CLKCONCMD &= ~0x40;
    while(CLKCONSTA & 0x40);
    CLKCONCMD &= ~0x47;

    PERCFG &= ~(1 << 0); //位置1 P0口
    P0SEL |= 0x0c; //P0.2和P0.3设置为外设功能
    P2DIR &= ~0xc0;

    U0CSR |= 0x80;
    U0GCR &= ~0x1F;
    U0GCR |= 8;
    U0BAUD = 59; //波特率设置为9600
    UTX0IF = 1; //发送中断标志位
    U0CSR |= 0x40; //允许接收
    IEN0 |= 0x84; //开总中断和串口0接收中断
}
/***************************************
* description: 串口发送字符串函数
* parameter: *Data, len
* return: none
* date: 2023-7-5
***************************************/
void Uart0_Send_String(uchar *Data, uint len)
{
    uint i;
    for(i = 0; i < len; i++)
    {
        U0DBUF = Data[i];
        
        while(UTX0IF == 0);
		UTX0IF = 0;
    }
}
/***************************************
*/
#pragma vector = URX0_VECTOR
__interrupt void Uart0_ISR()
{
    URX0IF = 0; //清除接收中断标志位
    rxTemp = U0DBUF;
}