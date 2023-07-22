/***************************************
* copyright:jing
* filename:main.c
* description:中断控制LED灯闪烁
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

uchar key = 0; //按键状态
//函数声明
void Delay_Ms(uint); //延时函数
void InitIO(); //初始化 LED 控制 IO 口函数
void InitKey(); //初始化按键 IO 口函数
void Interrupt_Init(); //中断初始化函数
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
    P1SEL &= ~((1<<5)|(1<<6)); //端口选择寄存器，0为通用IO口，1为专用功能
    P1DIR &= ~((1<<5)|(1<<6)); //端口方向寄存器，0为输入，1为输出
    P1INP &= ~((1<<5)|(1<<6)); //端口输入模式寄存器，0为上拉/下拉，1为三态
    P2INP &= ~(1<<6); //端口1上拉/下拉选择，0为上拉，1为下拉
}
/**************************************
* description:中断初始化函数
* parameters:无
* return value:无
* date:2023-7-4
**************************************/
void Interrupt_Init()
{
    PICTL |= (1<<2); //中断触发沿控制,下降沿触发
    P1IFG = 0x00; //中断状态标志复位
    P1IEN |= ((1<<5)|(1<<6)); //端口P1_5、P1_6中断使能
    P1IF = 0; //中断标志未挂起
    IEN2 |= (1<<4); //端口1中断使能
    EA = 1; //总中断使能
}
/**************************************
* description:main函数
* parameters:无
* return value:无
* date:2023-7-3
**************************************/
void main()
{
    InitIO(); //初始化 LED 灯控制 IO 口
    InitKey(); //初始化按键 IO 口
    Interrupt_Init();
    while(1) //死循环
    {
        if(key)
        {
            if(5 == key)
            {
                Delay_Ms(10);
                if(DOWN == SW1)
                {
                    LED1 = ON;
                }
            }
            else
            {
                Delay_Ms(10);
                if(DOWN == SW2)
                {
                    LED1 = OFF;
                }
            }
        }
        key = 0;
        Delay_Ms(150);
    }
}

/***************************************
* description:中断服务函数
* parameters:无
* return value:无
* date:2023-7-4
***************************************/
#pragma vector = P1INT_VECTOR
__interrupt void key_ISR()
{
    if(P1IFG & (1<<5))
    {
        key = 5;
        P1IFG &= ~(1<<5);
    }
    else if(P1IFG & (1<<6))
    {
        key = 6;
        P1IFG &= ~(1<<6);
    }
    P1IF = 0; //中断标志位，必须手动清零
}