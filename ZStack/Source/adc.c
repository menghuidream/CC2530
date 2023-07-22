#include "adc.h"

/***************************************
* description:ADC init
* parameter: none
* return: none
* date:2023-7-4
***************************************/
void adc_init()
{
    P0SEL |= (1<<7); //P0_7 ����Ϊ ADC ����
    P0DIR &= ~(1<<7); //P0_7 ����Ϊ����
    APCFG |= (1<<7); //ģ��IO P0_7 ʹ��

    ADCCON3 = 0xb7; //1011 0111,�ο���ѹΪAVDD5,��ȡ��Ϊ512��12λ������ͨ��ѡ��AIN7
    ADCCON1 |= ((1<<4)|(1<<5));//����ת��
    ADCH &= 0x00; //����߰�λ,�����־λEOC
}

/***************************************
* description:init IO
* parameter: none
* return: none
* date:2023-7-4
***************************************/
/*
void InitIO(void)
{
    P1SEL &= ~0x10; //P14 ����Ϊͨ�� IO ��
    P1DIR |= 0x10; //P14 ����Ϊ���
    LED1 = OFF; //LED1 ��
}*/

unsigned int getadc(void)
{
    unsigned int value;
    adc_init();//ADC ��ʼ��
	value = 0;
	APCFG |= (1<<7); //ģ��IO P0_7 ʹ��
	ADCCON1 |= (1<<6);//����ת��
	while(!(ADCCON1 & (1<<7))); //�ȴ�ת����� EOC = 1
	APCFG &= ~(1<<7); //ģ��IO P0_7 ����
	value = ADCL;//��ȡ�Ͱ�λ
	value |= (((int)ADCH)<<8);//��ȡ�߰�λ
	value >>= 4;//����4λ���õ�12λ��Ч����
	value = value & ~(1<<11);//�����12λ����λ���õ�11λ��Ч����
    return value;
}