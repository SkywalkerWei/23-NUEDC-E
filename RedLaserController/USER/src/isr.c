///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		isr
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ790875685)
// * @version    		查看doc内version文件 版本说明
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32G12K128
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"

uint8 uart_interrupt_flag = 0;
uint8 my_rx_buffer;//接收缓存
uint8 rx_buff[50];      //uart_rx数据缓冲区
#define PACK_HEAD 0xA5
#define PACK_TAIL 0xA6
char cnt=0;              //接收数据位
char bbsif[5];
uint8 bool_buff[5];
char byte_buff[10];
short short_buff[5];
uint8  int_buff[5];       //int数据保存区
float float_buff[10];    //float数据保存区
uint8 data_pack_flag=0;
int move_flag=0;
////////////////////////////////
void data_pack_Init(char b1,char b2,char s3,char i4,char f5)
{   		
		bbsif[0]=b1;					//蓝牙传输数据格式Bool->Byte->Short->Int->Float
		bbsif[1]=b2;
		bbsif[2]=s3;
		bbsif[3]=i4;
		bbsif[4]=f5;	
}	


void data_pack_receive()
{
	/*先判断帧头，如果帧头正确则继续接收数据至接收数组，
	待数据接收完之后再进行解包，将解包数据存至各类型数据的保存数组*/

		rx_buff[cnt] = my_rx_buffer;//将数据取出
		
		if(cnt==0 && rx_buff[0]!=PACK_HEAD)
		{
			return;
		}
		else if(rx_buff[cnt]==PACK_HEAD && cnt!=0)
		{
			cnt=1;
			rx_buff[0]=PACK_HEAD;
			return;
		}
		if(rx_buff[cnt]==PACK_TAIL)
		{
			/*int sum=0;                                //数据校验
			for(int i=1;i<cnt-1;i++)
			{
				sum+=rx_buff[i];										
			}
			if(sum%0x100!=rx_buff[cnt-1])
			{ 
				cnt=0;
				return;           //若数据校验位不合理，返回
			}*/
			
			uint8 buffsum=0;             //数据处理
			
			uint8 buff_bool=0;              //int数据保存位
			uint8 buff_byte=0;              //float数据保存位
			uint8 buff_short=0;              //float数据保存位				
			uint8 buff_int=0;              //int数据保存位
			uint8 buff_float=0;              //float数据保存位
      int i;
			 
			buffsum+=1;
			if(bbsif[0])
			{ 
				for( i=0;i<bbsif[0];i++)
				{						
					bool_buff[buff_bool+i] = rx_buff[buffsum]&(0x01<<i);
				}
					buffsum+=1;
			}						
			if(bbsif[1])
			{
				for( i=0;i<bbsif[1];i++)
				{			
          byte_buff[buff_byte+i] = rx_buff[buffsum];					
					buffsum+=1;
				}
				//move_flag=1;
			}						
			if(bbsif[2])
			{
				for( i=0;i<bbsif[2];i++)
				{						
					((uint8 *)short_buff)[buff_short+i*2] = rx_buff[buffsum];
					((uint8 *)short_buff)[buff_short+i*2+1] = rx_buff[buffsum+1];
					buffsum+=2;
				}
			}					
			if(bbsif[3])
			{
				for( i=0;i<bbsif[3];i++)
				{						
					((uint8 *)int_buff)[buff_int+i*4] = rx_buff[buffsum];
					((uint8 *)int_buff)[buff_int+i*4+1] = rx_buff[buffsum+1];
					((uint8 *)int_buff)[buff_int+i*4+2] = rx_buff[buffsum+2];
					((uint8 *)int_buff)[buff_int+i*4+3] = rx_buff[buffsum+3];
					buffsum+=4;
				}
			}
			if(bbsif[4])
			{
				for( i=0;i<bbsif[4];i++)
				{						
					((uint8 *)float_buff)[buff_float+i*4+0] = rx_buff[buffsum];
					((uint8 *)float_buff)[buff_float+i*4+1] = rx_buff[buffsum+1];
					((uint8 *)float_buff)[buff_float+i*4+2] = rx_buff[buffsum+2];
					((uint8 *)float_buff)[buff_float+i*4+3] = rx_buff[buffsum+3];
					buffsum+=4;
				}
			}
			cnt=0;
			data_pack_flag=1;		
			move_flag=1;
		}	
		
		if(cnt>50)
		{              //限制传输长度，可增大
			cnt=0;
		}
		else
		{
			cnt++;
		}				
}


//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF

	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断
uint8 res;
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
	   
    if(UART4_GET_RX_FLAG)
	{
		    
//			  data_pack_Init(0,10,0,0,0);
		    uart_interrupt_flag = 1;
			  my_rx_buffer=S4BUF;							 //读取数据
        data_pack_receive();
		
        
		    
		//接收数据寄存器为：S4BUF;
//		if(wireless_type == WIRELESS_SI24R1)
//        {
//            wireless_uart_callback();           //无线转串口回调函数
//        }
//        else if(wireless_type == WIRELESS_CH9141)
//        {
//            bluetooth_ch9141_uart_callback();   //蓝牙转串口回调函数
//        }
//        else if(wireless_type == WIRELESS_CH573)
//        {
//            wireless_ch573_callback();          //CH573无线模块回调函数
//        }
				UART4_CLEAR_RX_FLAG;
	}
	
}

#define LED P52
void INT0_Isr() interrupt 0
{
	LED = 0;	//点亮LED
}
void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}

void TM0_Isr() interrupt 1
{

}
void TM1_Isr() interrupt 3
{

}
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //清除中断标志
	
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
	
}


void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //清除中断标志

    
//	ccd_collect();	 //CCD采集数据

}

//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;