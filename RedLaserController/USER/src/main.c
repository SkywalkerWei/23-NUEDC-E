/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
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
 *在board_init中,已经将P54引脚设置为复位
 *如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */

#define DIR P35
int16 dat = 0;
extern res;
#define key1_flag P70
#define key2_flag P36
#define key3_flag P37
#define key4_flag P73
#define key5_flag P44 
#define key6_flag P51 //拨码开关
#define key7_flag P50 //拨码开关

uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;
uint8 key5_status = 1;


//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;
uint8 key5_last_status;

//开关标志位
uint8 flag1=1;
uint8 flag2=1;
uint8 flag3=1;
uint8 flag4=1;
uint8 flag5=1;


 



int N = 50; //总步数
int i;
float dx, dy; //计算XY轴位移量
float dxx, dyy; //每步位移量 ?
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
 //计算XY轴位移量
dx = pwmx1-pwmx2;
dy = pwmy1-pwmy2; 
 //计算每步位移量
 dxx = dx/N;
 dyy = dy/N;
	
for( i=0; i<N; i++) {

	if(key7_flag==0)
	{
		pwm_duty(PWMA_CH1P_P60,600);// Y   左边 上面     大向下 向左
		pwm_duty(PWMA_CH2P_P62,703);// X   右边 下面
		break;
	}
	if(key6_flag==0)
	{
		i=i-1;
		pwm_duty(PWMA_CH2P_P62, pwmx2+i*dxx); 
    pwm_duty(PWMA_CH1P_P60, pwmy2+i*dyy);
	}
	else
 //生成PWM信号控制x轴转动
{   pwm_duty(PWMA_CH2P_P62, pwmx2+i*dxx); 

 //生成PWM信号控制y轴转动
    pwm_duty(PWMA_CH1P_P60, pwmy2+i*dyy);
}
//延时等待位移完成
delay_ms(20); 
 }

}
void frame_deal_F()
{
	  pwm_duty(PWMA_CH1P_P60,600);//Y      左边 上面     大向下 向左         
		pwm_duty(PWMA_CH2P_P62,703);//X      右边 下面
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
// 原点  698 620
// 左上  778 556
// 右上  620 561
// 右下  616 692
// 左下  778 692
void frame_deal_S()
{
	  pwm_duty(PWMA_CH1P_P60,600);//Y      左边 上面     大向下 向左         
		pwm_duty(PWMA_CH2P_P62,703);//X      右边 下面
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

// 左上  736 620
// 右上  641 620
// 右下  641 684
// 左下  736 620



int a,b,c,d;
void main()
{
    WTST = 0;               //设置程序代码等待参数，赋值为0可将CPU执行程序的速度设置为最快
    
	DisableGlobalIRQ();		//关闭总中断
	
    //sys_clk可选值:35000000，30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600。
    //设置系统频率，此频率需要跟STC-ISP软件中的 <输入用户程序运行的IRC频率>选项的频率一致。
    //如果频率设置不对，将会导致串口的数据不正常,PWM的工作不正常等等。
    sys_clk = 35000000;     //设置系统频率为35MHz
    
	board_init();			//初始化寄存器
	//此处编写用户代码(例如：外设初始化代码等)
  uart_init(UART_4,UART4_RX_P02,UART4_TX_P03,9600,TIM_2);
  //data_pack_Init(0,10,0,0,0);
	oled_init();
	data_pack_Init(0,10,0,0,0);
  pwm_init(PWMA_CH1P_P60, 50, 0);     //初始化PWMA  使用引脚P6.0  输出PWM频率10000HZ   占空比为百分之 pwm_duty / PWM_DUTY_MAX * 100
	pwm_init(PWMA_CH2P_P62, 50, 0); 
	
	//总中断最后开启
	EnableGlobalIRQ();		//开启总中断
	
//	moveServo(6, 2000, 1000); //1秒移动1号舵机至2000位置
//	moveServo(2, 2000, 500);	
//	delay_ms(3000);
//	runActionGroup(1, 1); //运行1号动作组1次
//	runActionGroup(2, 1);
//	delay_ms(1500);
//  stopActionGroup();  //停止动作组运行
    while(1)
	{
	  key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
	  key5_last_status = key5_status;
    //读取当前按键状态
    key1_status = key1_flag;
    key2_status = key2_flag;
    key3_status = key3_flag;
    key4_status = key4_flag;
	  key5_status = key5_flag;

    //检测到按键按下之后  并放开置位标志位
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
					 pwm_duty(PWMA_CH1P_P60,600);// Y   左边 上面     大向下 向左
		       pwm_duty(PWMA_CH2P_P62,703);// X   右边 下面
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
//1个像素点20占空比


