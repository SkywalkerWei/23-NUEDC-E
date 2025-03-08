///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,��ɿƼ�
// * All rights reserved.
// * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
// *
// * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
// * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
// *
// * @file       		isr
// * @company	   		�ɶ���ɿƼ����޹�˾
// * @author     		��ɿƼ�(QQ790875685)
// * @version    		�鿴doc��version�ļ� �汾˵��
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32G12K128
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"

uint8 uart_interrupt_flag = 0;
uint8 my_rx_buffer;//���ջ���
uint8 rx_buff[50];      //uart_rx���ݻ�����
#define PACK_HEAD 0xA5
#define PACK_TAIL 0xA6
char cnt=0;              //��������λ
char bbsif[5];
uint8 bool_buff[5];
char byte_buff[10];
short short_buff[5];
uint8  int_buff[5];       //int���ݱ�����
float float_buff[10];    //float���ݱ�����
uint8 data_pack_flag=0;
int move_flag=0;
////////////////////////////////
void data_pack_Init(char b1,char b2,char s3,char i4,char f5)
{   		
		bbsif[0]=b1;					//�����������ݸ�ʽBool->Byte->Short->Int->Float
		bbsif[1]=b2;
		bbsif[2]=s3;
		bbsif[3]=i4;
		bbsif[4]=f5;	
}	


void data_pack_receive()
{
	/*���ж�֡ͷ�����֡ͷ��ȷ����������������������飬
	�����ݽ�����֮���ٽ��н������������ݴ������������ݵı�������*/

		rx_buff[cnt] = my_rx_buffer;//������ȡ��
		
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
			/*int sum=0;                                //����У��
			for(int i=1;i<cnt-1;i++)
			{
				sum+=rx_buff[i];										
			}
			if(sum%0x100!=rx_buff[cnt-1])
			{ 
				cnt=0;
				return;           //������У��λ����������
			}*/
			
			uint8 buffsum=0;             //���ݴ���
			
			uint8 buff_bool=0;              //int���ݱ���λ
			uint8 buff_byte=0;              //float���ݱ���λ
			uint8 buff_short=0;              //float���ݱ���λ				
			uint8 buff_int=0;              //int���ݱ���λ
			uint8 buff_float=0;              //float���ݱ���λ
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
		{              //���ƴ��䳤�ȣ�������
			cnt=0;
		}
		else
		{
			cnt++;
		}				
}


//UART1�ж�
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
        //�����Զ�����
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

//UART2�ж�
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
		//�������ݼĴ���Ϊ��S2BUF

	}
}


//UART3�ж�
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
		//�������ݼĴ���Ϊ��S3BUF

	}
}


//UART4�ж�
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
			  my_rx_buffer=S4BUF;							 //��ȡ����
        data_pack_receive();
		
        
		    
		//�������ݼĴ���Ϊ��S4BUF;
//		if(wireless_type == WIRELESS_SI24R1)
//        {
//            wireless_uart_callback();           //����ת���ڻص�����
//        }
//        else if(wireless_type == WIRELESS_CH9141)
//        {
//            bluetooth_ch9141_uart_callback();   //����ת���ڻص�����
//        }
//        else if(wireless_type == WIRELESS_CH573)
//        {
//            wireless_ch573_callback();          //CH573����ģ��ص�����
//        }
				UART4_CLEAR_RX_FLAG;
	}
	
}

#define LED P52
void INT0_Isr() interrupt 0
{
	LED = 0;	//����LED
}
void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //����жϱ�־
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //����жϱ�־
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //����жϱ�־
}

void TM0_Isr() interrupt 1
{

}
void TM1_Isr() interrupt 3
{

}
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //����жϱ�־
	
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //����жϱ�־
	
}


void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //����жϱ�־

    
//	ccd_collect();	 //CCD�ɼ�����

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