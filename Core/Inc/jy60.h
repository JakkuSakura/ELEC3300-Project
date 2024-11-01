#pragma once

#include <stdint.h>

// ax =((AxH<<8)|AxL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )
// ay =((AyH<<8)|AyL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )
// az =((AzH<<8)|AzL)/32768*16g(g 为重力加速度，可取 9.8m/s2 )

float get_acceleration(uint8_t AxH, uint8_t AxL) {
    return ( (((int16_t)AxH << 8) | (int16_t)AxL) / 32768 * 16) * 9.8;
}

// wx =((wxH<<8)|wxL)/32768*2000(°/s)
// wy =((wyH<<8)|wyL)/32768*2000(°/s)
// wz=((wzH<<8)|wzL)/32768*2000(°/s)

float get_angular_velocity(uint8_t wxH, uint8_t wxL) {
    return ((((int16_t)wxH << 8) | (int16_t)wxL) / 32768 * 2000);
}

// 滚转角（x 轴）Roll=((RollH<<8)|RollL)/32768*180(°)
// 俯仰角（y 轴）Pitch=((PitchH<<8)|PitchL)/32768*180(°)
// 偏航角（z 轴）Yaw=((YawH<<8)|YawL)/32768*180(°)

float get_angle(uint8_t RollH, uint8_t RollL) {
    return (((int16_t)RollH << 8) | (int16_t)RollL) / 32768 * 180.0;
}

