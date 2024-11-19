#pragma once

#include <stdint.h>
#include <math.h>


char YAWCMD[3] = {0XFF, 0XAA, 0X52};
char ACCCMD[3] = {0XFF, 0XAA, 0X67};
char SLEEPCMD[3] = {0XFF, 0XAA, 0X60};
char UARTMODECMD[3] = {0XFF, 0XAA, 0X61};
char IICMODECMD[3] = {0XFF, 0XAA, 0X62};

// ax =((AxH<<8)|AxL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )
// ay =((AyH<<8)|AyL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )
// az =((AzH<<8)|AzL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )

static inline float get_acceleration(uint8_t AxH, uint8_t AxL) {
    return ((((int16_t) AxH << 8) | (int16_t) AxL) / 32768 * 16) * 9.8;
}

// wx =((wxH<<8)|wxL)/32768*2000(°/s)
// wy =((wyH<<8)|wyL)/32768*2000(°/s)
// wz=((wzH<<8)|wzL)/32768*2000(°/s)

static inline float get_angular_velocity(uint8_t H, uint8_t L) {
    int16_t value_signed = (((uint16_t) H << 8) | (uint16_t) L);

    return value_signed / 32768.0f * 2000.0f;
}

// 滚转角（x 轴）Roll=((RollH<<8)|RollL)/32768*180(°)
// 俯仰角（y 轴）Pitch=((PitchH<<8)|PitchL)/32768*180(°)
// 偏航角（z 轴）Yaw=((YawH<<8)|YawL)/32768*180(°)

static inline float get_angle(uint8_t H, uint8_t L) {
    int16_t value_signed = (((uint16_t) H << 8) | (uint16_t) L);
    return value_signed / 32768.0f * 180.0f;
}

static inline float get_temperature(uint8_t H, uint8_t L) {
    int16_t value_signed = (((uint16_t) H << 8) | (uint16_t) L);
    return value_signed / 340.0f + 36.53f;
}

//用串口2给JY模块发送指令
static inline void sendcmd(char cmd[]) {
    HAL_UART_Transmit(&huart3, (uint8_t *) cmd, 3, 1000);
}

static inline void init_jy60() {
// vertical install
    sendcmd(IICMODECMD);
//    printf("正在进行加速度校准\r\n");
//    sendcmd(ACCCMD);//等待模块内部自动校准好，模块内部会自动计算需要一定的时间
//    printf("加速度校准完成\r\n");
//    HAL_Delay(100);
//    printf("进行Z轴角度清零\r\n");
    sendcmd(YAWCMD);
    HAL_Delay(100);
//    printf("Z轴角度清零完成\r\n");
}

static inline void process_packet(uint8_t *packet, StateRotation *rotation, StateRotation *rotation_offset,
                                  StateAngularVelocity *ang_vec) {
    uint8_t type = packet[1];
    switch (type) {
        case 0x51: {
            // acceleration
            uint8_t sum = packet[10];
            uint8_t computed_sum = 0;
            for (int i = 0; i < 10; ++i)
                computed_sum += packet[i];
            if (computed_sum != sum)
                break;
            break;
        }
        case 0x52: {
            // angular velocity
            uint8_t wxL = packet[2];
            uint8_t wxH = packet[3];
            uint8_t wyL = packet[4];
            uint8_t wyH = packet[5];
            uint8_t wzL = packet[6];
            uint8_t wzH = packet[7];
            uint8_t sum = packet[10];
            uint8_t computed_sum = 0;
            for (int i = 0; i < 10; ++i)
                computed_sum += packet[i];
            if (computed_sum != sum)
                break;
            ang_vec->roll = get_angular_velocity(wyH, wyL);
            ang_vec->pitch = -get_angular_velocity(wxH, wxL);
            ang_vec->yaw = get_angular_velocity(wzH, wzL);
            break;
        }
        case 0x53: {
            // angle
            uint8_t rollL = packet[2];
            uint8_t rollH = packet[3];
            uint8_t pitchL = packet[4];
            uint8_t pitchH = packet[5];
            uint8_t yawL = packet[6];
            uint8_t yawH = packet[7];
            uint8_t tempL = packet[8];
            uint8_t tempH = packet[9];
            uint8_t sum = packet[10];
            uint8_t computed_sum = 0;
            for (int i = 0; i < 10; ++i)
                computed_sum += packet[i];
//            if (computed_sum != sum)
//            	break;
            StateRotation rot_temp;

            rot_temp.roll = get_angle(pitchH, pitchL) - rotation_offset->roll;
//            if (fabs(rot_temp.roll - rotation->roll) < 100040.0f) {
                rotation->roll = rot_temp.roll;
//            }
            rot_temp.pitch = -get_angle(rollH, rollL) - rotation_offset->pitch;
//            if (fabs(rot_temp.pitch - rotation->pitch) < 100040.0f) {
                rotation->pitch = rot_temp.pitch;
//            }
            rot_temp.yaw = get_angle(yawH, yawL) - rotation_offset->yaw;
//            if (fabs(rot_temp.yaw - rotation->yaw) < 100040.0f) {
                rotation->yaw = rot_temp.yaw;
//            }
            rotation->temp = get_temperature(tempH, tempL);
            break;
        }
    }
}

#define BUFFER_SIZE 11
#define MAX_READ_SIZE 32 // Maximum bytes you can read at once
#define TOTAL_BUFFER_SIZE 256

static inline void parse_jy60(uint8_t *data, uint16_t len, StateRotation *rotation, StateRotation *rotation_offset,
                              StateAngularVelocity *ang_vec) {
    static uint8_t buffer[TOTAL_BUFFER_SIZE];
    static uint8_t index = 0;
    // Append new data to the buffer if there's enough space
    if (index + len > TOTAL_BUFFER_SIZE) {
        // Handle overflow by discarding excess data
        index = 0; // Optionally reset the index here
    }

    // Append new data
    memcpy(&buffer[index], data, len);
    index += len;

    // Process packets
    for (uint8_t i = 0; i <= index - BUFFER_SIZE;) {
        if (buffer[i] == 0x55 && (buffer[i + 1] == 0x51 || buffer[i + 1] == 0x52 || buffer[i + 1] == 0x53)) {
            // Found a start of a packet
            if (i + BUFFER_SIZE <= index) {
                // Process the complete packet
                process_packet(&buffer[i], rotation, rotation_offset, ang_vec);
                i += BUFFER_SIZE;
            } else {
                memmove(&buffer[0], &buffer[i], index - i);
                index -= i;
                break;
            }
        } else {
            i += 1;
        }
    }
}
