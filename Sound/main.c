#include <ioCC2530.h>
#include <string.h>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1000; j++) {
            // ��ʱԼ1ms
        }
    }
}

void SYN_FrameInfo(unsigned char Music,unsigned char *HZdata);
void Uart1_Init();
void Uart1_Send_String(unsigned char *Data, unsigned char len);

void main(void) {
	Uart1_Init();

    while (1) {
		SYN_FrameInfo(0, "�������ӿƼ�");
		delay_ms(10000);
	}
}

void Uart1_Init()
{
	CLKCONCMD &= ~0x40;
	while(CLKCONSTA & 0x40);
	CLKCONCMD &= ~0x47;

	PERCFG &= ~(1 << 0); //λ��1 P0��
	P0SEL |= 0x30; //P0.2��P0.3����Ϊ���蹦��
	P2DIR &= ~0xc0;

	U1CSR |= 0x80;
	U1GCR &= ~0x1F;
	U1GCR |= 8;
	U1BAUD = 59; //����������Ϊ9600
	UTX1IF = 1; //�����жϱ�־λ
	U1CSR |= 0x40; //�������
	IEN0 |= 0x84; //�����жϺʹ���0�����ж�
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
* ��    �ƣ�  YS-SYN6288 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���SYN6288оƬ���кϳɲ���
* ��ڲ�����Music(��������ѡ��):0�ޱ������֡�1-15����ر�������
            *HZdata:�ı�ָ����� 
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��					 
* ���÷��������� SYN_FrameInfo��0�����������ӿƼ�������
**********************************************************/
void SYN_FrameInfo(unsigned char Music,unsigned char *HZdata)
{
/****************��Ҫ���͵��ı�**********************************/ 
		 unsigned  char  Frame_Info[200];
         unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  			//����У���ֽ�
	     unsigned  int i=0; 
		 HZ_Length =strlen(HZdata); 			//��Ҫ�����ı��ĳ���
 
/*****************֡�̶�������Ϣ**************************************/           
		 Frame_Info[0] = 0xFD ; 			//����֡ͷFD
		 Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
		 Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
		 Frame_Info[4] = 0x01 | Music<<4 ;  //����������������������趨

/*******************У�������***************************************/		 
		 for(i = 0; i<5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	     {  
	         ecc=ecc^(Frame_Info[i]);		//�Է��͵��ֽڽ������У��	
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
	     {  
	         ecc=ecc^(HZdata[i]); 				//�Է��͵��ֽڽ������У��		
	     }		 
/*******************����֡��Ϣ***************************************/		  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  Uart1_Send_String(Frame_Info,5+HZ_Length+1);
}
