/***************************************
* copyright:jing
* filename:main.c
* description:ADC test
* author:jing
* phone:***********
* version:1.0
* date:2023-7-4
***************************************/

#include <ioCC2530.h>
#include <stdio.h>
#define uint unsigned int
#define LED1 P1_4 //LED1
#define ON 0 //LED1 亮
#define OFF 1 //LED1 灭

void clk_32mset(); //32M clock set
void adc_init(); //ADC init
void Delay_Ms(uint); //delay function
void InitIO(); //init IO
/***************************************
* description:main
* parameter: none
* return: none
* date:2023-7-4
***************************************/
void main()
{
    int value;
    clk_32mset();
    adc_init();
    InitIO();
    while(1)
    {
        adc_init();//ADC 初始化
        value = 0;
        APCFG |= (1<<7); //模拟IO P0_7 使能
        ADCCON1 |= (1<<6);//启动转换
        while(!(ADCCON1 & (1<<7))); //等待转换完成 EOC = 1
        APCFG &= ~(1<<7); //模拟IO P0_7 禁用
        value = ADCL;//读取低八位
        value |= (((int)ADCH)<<8);//读取高八位
        value >>= 4;//右移4位，得到12位有效数据
        value = value & ~(1<<11);//清除第12位符号位，得到11位有效数据
        printf("value = %d\n",value);//打印结果
        if (value > 1000)//判断结果
        {
            LED1 = ON;
        }
        else
        {
            LED1 = OFF;
        }
        Delay_Ms(1000);
    }
}
/***************************************
* description:32M clock set
* parameter: none
* return: none
* date:2023-7-4
***************************************/
void clk_32mset()
{
    CLKCONCMD &= ~(1<<6); //设置系统时钟源为32MHz
    while(CLKCONSTA & (1<<6)); //等待晶振稳定
    CLKCONCMD &= ~0x07; //设置系统主时钟为32MHz
}
/***************************************
* description:ADC init
* parameter: none
* return: none
* date:2023-7-4
***************************************/
void adc_init()
{
    P0SEL |= (1<<7); //P0_7 定义为 ADC 输入
    P0DIR &= ~(1<<7); //P0_7 定义为输入
    APCFG |= (1<<7); //模拟IO P0_7 使能

    ADCCON3 = 0xb7; //1011 0111,参考电压为AVDD5,抽取率为512（12位），单通道选择AIN7
    ADCCON1 |= ((1<<4)|(1<<5));//启动转换
    ADCH &= 0x00; //清除高八位,清楚标志位EOC
}
/***************************************
* description:delay function
* parameter: ms
* return: none
* date:2023-7-4
***************************************/
void Delay_Ms(uint n)
{
    uint i, j;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < 120; j++);
    }
}
/***************************************
* description:init IO
* parameter: none
* return: none
* date:2023-7-4
***************************************/
void InitIO(void)
{
    P1SEL &= ~0x10; //P14 定义为通用 IO 口
    P1DIR |= 0x10; //P14 定义为输出
    LED1 = OFF; //LED1 灭
}