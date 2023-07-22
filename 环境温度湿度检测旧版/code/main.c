/***************************************
* copyright: jing
* filename: main.c
* description: Temperature and Humidity
* author: jing
* phone: ***********
* version: 1.0
* date: 2023-7-6
***************************************/
#include <stdio.h>
#include "hal_dht11.h"
#include "hal_delay.h"
#include <ioCC2530.h>
#include <string.h>
#include "codetab.h"
#include "LQ12864.h"
#define uint unsigned int
#define uchar unsigned char

void Beep_OFF(); //蜂鸣器关闭函数声明
void setSystemClk32MHZ(); //设置系统时钟为32MHZ

void main()
{
    Beep_OFF();

	OLED_Init();//初始化OLED
	OLED_CLS();//清屏

    halDHT11Data_t dht11Dat;//定义一个结构体变量,用于存放温度和湿度
    uchar tempStr[50], humiStr[50];//定义两个字符串数组,用于存放温度和湿度的字符串

    setSystemClk32MHZ();//设置系统时钟为32MHZ
    halDHT11Init();//初始化DHT11
    while(1)
    {
        dht11Dat = halDHT11GetData();//获取温度和湿度
        if (dht11Dat.ok) {
            sprintf((char *)tempStr, "Temp: %d", dht11Dat.temp);//将温度转换为字符串
            sprintf((char *)humiStr, "Humi: %d", dht11Dat.humi);//将湿度转换为字符串

            printf("Temp: %d\n", dht11Dat.temp);//打印温度
            printf("Humi: %d\n", dht11Dat.humi);//打印湿度

            OLED_P8x16Str(0, 0, tempStr);//在OLED上显示温度
            OLED_P8x16Str(0, 2, humiStr);//在OLED上显示湿度
        }
        delayMs(SYSCLK_32MHZ, 4000);//延时4秒
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
* description: 设置系统时钟为32MHZ
* parameter: none
* return: none
* date: 2023-7-5
***************************************/
void setSystemClk32MHZ()
{
    CLKCONCMD &= ~0x40;//设置系统时钟源为32MHZ
    while(CLKCONSTA & 0x40);//等待系统时钟设置完成
    CLKCONCMD &= ~0x47;//设置系统时钟为32MHZ
}
