/***************************************
* copyright:jing
* filename:main.c
* description:IO口控制LED灯闪烁
* author:jing
* phone:***********
* version:1.0
* date:2023-7-3
***************************************/

#include <ioCC2530.h>
#define uint unsigned int
#define uchar unsigned char

//定义控制灯的端口
#define LED1 P1_4 //定义 LED1 为 P1_4 口控制
#define LED2 P0_1 //定义 LED2 为 P0_1 口控制
#define LED3 P1_0 //定义 LED3 为 P1_0 口控制
#define LED4 P1_1 //定义 LED4 为 P1_1 口控制
#define ON 0 
#define OFF 1

//定义开关端口
#define SW1 P1_5
#define SW2 P1_6
#define DOWN 0
#define UP 1

uchar key = 0;//按键状态
uchar key5 = 0;//按键5状态
//函数声明
void Delay_Ms(uint); //延时函数
void InitIO(); //初始化 LED 控制 IO 口函数
void InitKey(); //初始化按键 IO 口函数
uchar KeyScan(); //按键扫描函数
/**************************************
* description:延时毫秒函数
* parameters:无
* return value:无
* date:2023-7-3
***************************************/
void Delay_Ms(uint n)
{
    uint i,j;
    for(i = 0;i<n;i++)
    {
        for(j = 0; j < 120; j++);
    }
}
/***************************************
* description:初始化 LED 控制 IO 口函数
* parameters:无
* return value:无
* date:2023-7-3
***************************************/
void InitIO(void)
{
    P0SEL &= ~0x02; //P01 定义为通用 IO 口
    P1SEL &= ~0x13; //P10、 P11、 P14 定义为通用 IO 口
    P0DIR |= 0x02; //P01 定义为输出
    P1DIR |= 0x13; //P10、 P11、 P14 定义为输出
    LED1 = OFF;
    LED2 = OFF;
    LED3 = OFF;
    LED4 = OFF; //LED 灯初始化为关
}
/**************************************
* description:初始化按键 IO 口函数
* parameters:无
* return value:无
* date:2023-7-4
**************************************/
void InitKey()
{
    P1SEL &= ~((1<<5)|(1<<6));
    P1DIR &= ~((1<<5)|(1<<6));
    P1INP &= ~((1<<5)|(1<<6));
    P2INP &= ~(1<<6);
}
/**************************************
* description:按键扫描函数,get key status
* parameters:无
* return value:  5:SW1 DOWN,6:SW2 DOWN,0:NO KEY DOWN
* date:2023-7-4
**************************************/
uchar KeyScan()
{
    if(DOWN == SW1)
    {
        Delay_Ms(10);
        if(DOWN == SW1)
        {
            key5 = 1;
            return 5;
        }
    }
    else if(DOWN == SW2)
    {
        Delay_Ms(10);
        if(DOWN == SW2)
        {
            return 6;
        }
    }
    return key;
}
/**************************************
* description:main函数
* parameters:无
* return value:无
* date:2023-7-3
**************************************/
void main(void)
{
    InitIO(); //初始化 LED 灯控制 IO 口
    InitKey(); //初始化按键 IO 口
    while(1) //死循环
    {
        key = KeyScan();
		Delay_Ms(100);
        switch(key){
            case 5:
                if(key5 == 1)
                {
                    LED1 = LED2;
                    LED2 = !LED1;
                    LED3 = OFF;
                    LED4 = OFF;
                    key5 = 0;
                }
                break;
            case 6:
                LED1 = !LED1;
                LED2 = OFF;
                LED3 = OFF;
                LED4 = OFF;
                break;
            default:
                LED1 = !LED1; // LED1 灯闪一次
                Delay_Ms(200);
                LED2 = !LED2; // LED2 灯闪一次
                Delay_Ms(200);
                LED3 = !LED3; // LED3 灯闪一次
                Delay_Ms(200);
                LED4 = !LED4; // LED4 灯闪一次
                Delay_Ms(200);
                break;
        }

        Delay_Ms(1000);
    }
}