#pragma once

#include <stdint.h>
#include <math.h>

uint8_t UNLOCKCMD[5] = {0XFF, 0XAA, 0X69, 0X88, 0XB5};
uint8_t SAVECMD[5] = {0XFF, 0XAA, 0X00, 0X00, 0X00};

uint8_t YAWCMD[3] = {0XFF, 0XAA, 0X52};
uint8_t ACCCMD[3] = {0XFF, 0XAA, 0X67};
uint8_t SLEEPCMD[3] = {0XFF, 0XAA, 0X60};
uint8_t UARTMODECMD[3] = {0XFF, 0XAA, 0X61};
uint8_t IICMODECMD[3] = {0XFF, 0XAA, 0X62};

uint8_t RRATE_FAST[5] = {0XFF, 0XAA, 0X03, 0X0B, 0X00}; // 200

int baud_rate = 230400;
char BAUDRATE_FAST[5] = {0XFF, 0XAA, 0X04, 0X07, 0X00}; // 230400

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


//用串口2给JY模块发送指令
static inline void sendcmd(uint8_t cmd[]) {
    HAL_UART_Transmit(&huart3, (uint8_t *) cmd, 3, 1000);
}

static inline void init_jy60() {
//    sendcmd(UNLOCKCMD);
//    sendcmd(BAUDRATE_FAST);
//    HAL_Delay(20);
//    huart3.Instance->BRR = UART_BRR_SAMPLING8(HAL_RCC_GetPCLK2Freq(), baud_rate);
//    sendcmd(SAVECMD);

    sendcmd(UNLOCKCMD);
// vertical install
    sendcmd(IICMODECMD);
    HAL_Delay(100);
//    printf("正在进行加速度校准\r\n");
//    sendcmd(ACCCMD);//等待模块内部自动校准好，模块内部会自动计算需要一定的时间
//    printf("加速度校准完成\r\n");
//    HAL_Delay(100);
//    printf("进行Z轴角度清零\r\n");
    sendcmd(YAWCMD);
    HAL_Delay(10);
    sendcmd(RRATE_FAST);
    HAL_Delay(10);
    sendcmd(SAVECMD);

}

static inline void process_packet(uint8_t *packet, StateRotation *rotation,
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
            break;
            // it does not work for some reason

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
            //if (computed_sum != sum)
            //    break;
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

            rotation->roll = -get_angle(pitchH, pitchL);

            rotation->pitch = 90.0f - get_angle(rollH, rollL);
            float yaw = -get_angle(yawH, yawL);

            rotation->yaw = yaw;

            break;
        }
    }
}

#define BUFFER_SIZE 11
#define MAX_READ_SIZE 32 // Maximum bytes you can read at once
#define TOTAL_BUFFER_SIZE 256

static inline void parse_jy60(uint8_t *data, uint16_t len, StateRotation *rotation,
                              StateAngularVelocity *ang_vec) {
    static uint8_t buffer[TOTAL_BUFFER_SIZE];
    static uint16_t index = 0;
    // Append new data to the buffer if there's enough space
    if (index + len > TOTAL_BUFFER_SIZE) {
        // Handle overflow by discarding excess data
        index = 0; // Optionally reset the index here
    }

    // Append new data
    memcpy(&buffer[index], data, len);
    index += len;

    // Process packets
    for (uint16_t i = 0; i <= index - BUFFER_SIZE;) {
        if (buffer[i] == 0x55) {
            // Found a start of a packet
            if (i + BUFFER_SIZE <= index) {
                // Process the complete packet
                process_packet(&buffer[i], rotation, ang_vec);
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
