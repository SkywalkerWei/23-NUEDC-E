/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
#include <math.h>
extern uint8 byte_buff[10];
extern int move_flag;
//extern void data_pack_Init(char b1,char b2,char s3,char i4,char f5);
/*
 *��board_init��,�Ѿ���P54��������Ϊ��λ
 *�����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */

#define DIR P35
int16 dat = 0;
extern res;
#define key1_flag P70
#define key2_flag P36
#define key3_flag P37
#define key4_flag P73
#define key5_flag P44 
#define key6_flag P51 //���뿪��
#define key7_flag P50 //���뿪��

uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;
uint8 key5_status = 1;


//��һ�ο���״̬����
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;
uint8 key5_last_status;

//���ر�־λ
uint8 flag1=1;
uint8 flag2=1;
uint8 flag3=1;
uint8 flag4=1;
uint8 flag5=1;


 



int N = 50; //�ܲ���
int i;
float dx, dy; //����XY��λ����
float dxx, dyy; //ÿ��λ���� ?
int ch,sh;
float k;

int PWMX1,PWMY1,PWMX2,PWMY2,PWMX3,PWMY3,PWMX4,PWMY4;

void get_pwmxy()
{
	PWMX1=(byte_buff[0]-byte_buff[8])*1.8+703;
	PWMY1=(byte_buff[9]-byte_buff[1])*1.8+602;

	PWMX2=(byte_buff[0]-byte_buff[6])*1.8+703;
	PWMY2=(byte_buff[7]-byte_buff[1])*1.8+602;

	PWMX3=(byte_buff[0]-byte_buff[4])*1.8+703;
	PWMY3=(byte_buff[5]-byte_buff[1])*1.8+600;

	PWMX4=(byte_buff[0]-byte_buff[2])*1.8+703;
	PWMY4=(byte_buff[3]-byte_buff[1])*1.8+600;
}


void moveServo1(int pwmx1, int pwmx2,int pwmy1, int pwmy2)
{
 //����XY��λ����
dx = pwmx1-pwmx2;
dy = pwmy1-pwmy2; 
 //����ÿ��λ����
 dxx = dx/N;
 dyy = dy/N;
	
for( i=0; i<N; i++) {

	if(key7_flag==0)
	{
		pwm_duty(PWMA_CH1P_P60,600);// Y   ��� ����     ������ ����
		pwm_duty(PWMA_CH2P_P62,703);// X   �ұ� ����
		break;
	}
	if(key6_flag==0)
	{
		i=i-1;
		pwm_duty(PWMA_CH2P_P62, pwmx2+i*dxx); 
    pwm_duty(PWMA_CH1P_P60, pwmy2+i*dyy);
	}
	else
 //����PWM�źſ���x��ת��
{   pwm_duty(PWMA_CH2P_P62, pwmx2+i*dxx); 

 //����PWM�źſ���y��ת��
    pwm_duty(PWMA_CH1P_P60, pwmy2+i*dyy);
}
//��ʱ�ȴ�λ�����
delay_ms(20); 
 }

}
void frame_deal_F()
{
	  pwm_duty(PWMA_CH1P_P60,600);//Y      ��� ����     ������ ����         
		pwm_duty(PWMA_CH2P_P62,703);//X      �ұ� ����
	  delay_ms(1000); 
		moveServo1(782,703,538,600);
		delay_ms(1000);
		moveServo1(623,782,547,538);
		delay_ms(1000);
		moveServo1(623,623,677,547);
		delay_ms(1000);
		moveServo1(782,623,671,677);
		delay_ms(1000);
		moveServo1(780,782,538,671);
		delay_ms(1000);
}
// ԭ��  698 620
// ����  778 556
// ����  620 561
// ����  616 692
// ����  778 692
void frame_deal_S()
{
	  pwm_duty(PWMA_CH1P_P60,600);//Y      ��� ����     ������ ����         
		pwm_duty(PWMA_CH2P_P62,703);//X      �ұ� ����
	  delay_ms(1000); 
		moveServo1(746,703,608,600);
		delay_ms(1000);
		moveServo1(656,746,608,608);
		delay_ms(1000);
		moveServo1(656,656,661,608);
		delay_ms(1000);
		moveServo1(746,656,661,661);
		delay_ms(1000);
		moveServo1(746,746,603,661);
		delay_ms(1000);
}

// ����  736 620
// ����  641 620
// ����  641 684
// ����  736 620



int a,b,c,d;
void main()
{
    WTST = 0;               //���ó������ȴ���������ֵΪ0�ɽ�CPUִ�г�����ٶ�����Ϊ���
    
	DisableGlobalIRQ();		//�ر����ж�
	
    //sys_clk��ѡֵ:35000000��30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600��
    //����ϵͳƵ�ʣ���Ƶ����Ҫ��STC-ISP����е� <�����û��������е�IRCƵ��>ѡ���Ƶ��һ�¡�
    //���Ƶ�����ò��ԣ����ᵼ�´��ڵ����ݲ�����,PWM�Ĺ����������ȵȡ�
    sys_clk = 35000000;     //����ϵͳƵ��Ϊ35MHz
    
	board_init();			//��ʼ���Ĵ���
	//�˴���д�û�����(���磺�����ʼ�������)
  uart_init(UART_4,UART4_RX_P02,UART4_TX_P03,9600,TIM_2);
  //data_pack_Init(0,10,0,0,0);
	oled_init();
	data_pack_Init(0,10,0,0,0);
  pwm_init(PWMA_CH1P_P60, 50, 0);     //��ʼ��PWMA  ʹ������P6.0  ���PWMƵ��10000HZ   ռ�ձ�Ϊ�ٷ�֮ pwm_duty / PWM_DUTY_MAX * 100
	pwm_init(PWMA_CH2P_P62, 50, 0); 
	
	//���ж������
	EnableGlobalIRQ();		//�������ж�
	
//	moveServo(6, 2000, 1000); //1���ƶ�1�Ŷ����2000λ��
//	moveServo(2, 2000, 500);	
//	delay_ms(3000);
//	runActionGroup(1, 1); //����1�Ŷ�����1��
//	runActionGroup(2, 1);
//	delay_ms(1500);
//  stopActionGroup();  //ֹͣ����������
    while(1)
	{
	  key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
	  key5_last_status = key5_status;
    //��ȡ��ǰ����״̬
    key1_status = key1_flag;
    key2_status = key2_flag;
    key3_status = key3_flag;
    key4_status = key4_flag;
	  key5_status = key5_flag;

    //��⵽��������֮��  ���ſ���λ��־λ
    if(key1_status && !key1_last_status)    flag1 = 0;
    if(key2_status && !key2_last_status)    flag2 = 0;
    if(key3_status && !key3_last_status)    flag3 = 0;	
    if(key4_status && !key4_last_status)    flag4 = 0;
	  if(key5_status && !key5_last_status)    flag5 = 0;
		
		
		 if(flag3==0)
		 {
			 flag3=1;
			 frame_deal_F();
		 }
		 if(flag1==0)
		 {
			 flag1=1;
			 frame_deal_S();
		 }
	
	  if(flag5==0)
				{
					 flag5=1;
					  uart_putchar(UART_4,0xA5);
				}
		if(flag4==0)
				{
					 flag4=1;
					 pwm_duty(PWMA_CH1P_P60,600);// Y   ��� ����     ������ ����
		       pwm_duty(PWMA_CH2P_P62,703);// X   �ұ� ����
					 delay_ms(10);
				}
		oled_int16(1,0,byte_buff[0]);
		oled_int16(1,1,byte_buff[1]);
		oled_int16(1,2,byte_buff[2]);
		oled_int16(1,3,byte_buff[3]);
		oled_int16(1,4,byte_buff[4]);
		oled_int16(1,5,byte_buff[5]);
		oled_int16(1,6,byte_buff[6]);
		oled_int16(1,7,byte_buff[7]);
		oled_int16(40,1,byte_buff[8]);
		oled_int16(40,2,byte_buff[9]);
    
		if(move_flag==1)
		{           //    -17*2   28 
//			k=2;
//			sh=580;//y
//			ch=680;//x   44,40  61,68    88,48
//			pwm_duty(PWMA_CH1P_P60,620);//530
//			pwm_duty(PWMA_CH2P_P62,624);//720
				get_pwmxy();
				moveServo1(PWMX1,703,PWMY1,600);
				delay_ms(1000);
			  moveServo1(PWMX2,PWMX1,PWMY2,PWMY1);
			  delay_ms(1000);
			  moveServo1(PWMX3,PWMX2,PWMY3,PWMY2);
			  delay_ms(1000);
				moveServo1(PWMX4,PWMX3,PWMY4,PWMY3);
			  delay_ms(1000);
			  moveServo1(PWMX1,PWMX4,PWMY1,PWMY4);
			  delay_ms(1000);
			  move_flag=0;
		}
		oled_int16(40,3,move_flag);
		oled_int16(40,4,sh);
		oled_int16(40,5,ch);	

    }
	
}
//1�����ص�20ռ�ձ�


