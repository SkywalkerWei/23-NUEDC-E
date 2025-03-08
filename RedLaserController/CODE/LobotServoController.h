/*******************************************************************************
* �ļ���: LobotServoController.h
* ����: �����ֻ������Ƽ�
* ���ڣ�20160806
* LSCϵ�ж�����ư���ο���ʾ��
*******************************************************************************/

#ifndef LOBOTSERVOCONTROLLER_H_
#define LOBOTSERVOCONTROLLER_H_

#include "headfile.h"



#define FRAME_HEADER 0x55             //֡ͷ
#define CMD_SERVO_MOVE 0x03           //����ƶ�ָ��
#define CMD_ACTION_GROUP_RUN 0x06     //���ж�����ָ��
#define CMD_ACTION_GROUP_STOP 0x07    //ֹͣ������ָ��
#define CMD_ACTION_GROUP_SPEED 0x0B   //���ö����������ٶ�
#define CMD_GET_BATTERY_VOLTAGE 0x0F  //��ȡ��ص�ѹָ��


extern uint8 LobotRxBuf[16];
extern uint16 batteryVolt;
extern void receiveHandle(void);

typedef struct _lobot_servo_ {  //���ID,���Ŀ��λ��
	uint8 ID;
	uint16 Position;
} LobotServo;


void moveServo(uint8 servoID, uint16 Position, uint16 Time);
void moveServos(uint8 Num, uint16 Time);
void runActionGroup(uint8 numOfAction, uint16 Times);
void stopActionGroup(void);
void setActionGroupSpeed(uint8 numOfAction, uint16 Speed);
void setAllActionGroupSpeed(uint16 Speed);
void getBatteryVoltage(void);



#endif
