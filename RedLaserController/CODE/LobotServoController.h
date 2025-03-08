/*******************************************************************************
* 文件名: LobotServoController.h
* 作者: 深圳乐幻索尔科技
* 日期：20160806
* LSC系列舵机控制板二次开发示例
*******************************************************************************/

#ifndef LOBOTSERVOCONTROLLER_H_
#define LOBOTSERVOCONTROLLER_H_

#include "headfile.h"



#define FRAME_HEADER 0x55             //帧头
#define CMD_SERVO_MOVE 0x03           //舵机移动指令
#define CMD_ACTION_GROUP_RUN 0x06     //运行动作组指令
#define CMD_ACTION_GROUP_STOP 0x07    //停止动作组指令
#define CMD_ACTION_GROUP_SPEED 0x0B   //设置动作组运行速度
#define CMD_GET_BATTERY_VOLTAGE 0x0F  //获取电池电压指令


extern uint8 LobotRxBuf[16];
extern uint16 batteryVolt;
extern void receiveHandle(void);

typedef struct _lobot_servo_ {  //舵机ID,舵机目标位置
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
