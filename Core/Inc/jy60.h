#pragma once

#include <stdint.h>

struct SAcc {
    short a[3];
    short T;
};
struct SGyro {
    short w[3];
    short T;
};
struct SAngle {
    short Angle[3];
    short T;
};

char YAWCMD[3] = {0XFF, 0XAA, 0X52};
char ACCCMD[3] = {0XFF, 0XAA, 0X67};
char SLEEPCMD[3] = {0XFF, 0XAA, 0X60};
char UARTMODECMD[3] = {0XFF, 0XAA, 0X61};
char IICMODECMD[3] = {0XFF, 0XAA, 0X62};

// ax =((AxH<<8)|AxL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )
// ay =((AyH<<8)|AyL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )
// az =((AzH<<8)|AzL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )

float get_acceleration(uint8_t AxH, uint8_t AxL) {
    return ((((int16_t) AxH << 8) | (int16_t) AxL) / 32768 * 16) * 9.8;
}

// wx =((wxH<<8)|wxL)/32768*2000(°/s)
// wy =((wyH<<8)|wyL)/32768*2000(°/s)
// wz=((wzH<<8)|wzL)/32768*2000(°/s)

float get_angular_velocity(uint8_t wxH, uint8_t wxL) {
    return ((((int16_t) wxH << 8) | (int16_t) wxL) / 32768 * 2000);
}

// 滚转角（x 轴）Roll=((RollH<<8)|RollL)/32768*180(°)
// 俯仰角（y 轴）Pitch=((PitchH<<8)|PitchL)/32768*180(°)
// 偏航角（z 轴）Yaw=((YawH<<8)|YawL)/32768*180(°)

float get_angle(uint8_t RollH, uint8_t RollL) {
    return (((int16_t) RollH << 8) | (int16_t) RollL) / 32768 * 180.0;
}

//用串口2给JY模块发送指令
void sendcmd(char cmd[]) {
    char i;
    for (i = 0; i < 3; i++)
        UART2_Put_Char(cmd[i]);
}

void init_jy60() {
    printf("正在进行加速度校准\r\n");
    sendcmd(ACCCMD);//等待模块内部自动校准好，模块内部会自动计算需要一定的时间
    printf("加速度校准完成\r\n");
    delay_ms(100);
    printf("进行Z轴角度清零\r\n");
    sendcmd(YAWCMD);
    printf("Z轴角度清零完成\r\n");
}


static inline void parse_jy60(uint8_t *data, uint16_t len) {

    // ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
    //	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
    //	{
    //		ucRxCnt=0;
    //		return;
    //	}
    //	if (ucRxCnt<11) {return;}//数据不满11个，则返回
    //	else
    //	{
    //		switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
    //		{
    //			//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
    //			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
    //			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
    //			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
    //
    //		}
    //		ucRxCnt=0;//清空缓存区
    //	}
}