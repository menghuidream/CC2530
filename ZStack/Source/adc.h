#include <ioCC2530.h>
#include <stdio.h>
#define uint unsigned int
#define LED1 P1_4 //LED1
#define ON 0 //LED1 ÁÁ
#define OFF 1 //LED1 Ãð

unsigned int getadc(void);
void adc_init(); //ADC init
//void InitIO(); //init IO