#pragma once

#include <stdio.h>
#include "usart.h"

void write_mouse(uint8_t left, uint8_t right, int8_t x, int8_t y) {
    static char buffer[64];
    sprintf(buffer, "Mouse %d %d %d %d\r\n", left, right, x, y);
    HAL_UART_Transmit(&huart1, (uint8_t *) buffer, strlen(buffer), 1000);
}