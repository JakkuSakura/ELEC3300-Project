#pragma once

#include <stdio.h>
#include "usart.h"
#include "state.h"

void write_mouse(uint8_t left, uint8_t right, int8_t x, int8_t y) {
    static char buffer[64];
    sprintf(buffer, "Mouse %d %d %d %d\r\n", left, right, x, y);
    HAL_UART_Transmit(&huart1, (uint8_t *) buffer, strlen(buffer), 1000);
}

void write_keyboard(uint8_t ctrl, uint8_t shift, uint8_t alt, const uint16_t *keys, uint8_t len) {
    static char buffer[128];
    int length = 0;
    length += sprintf(buffer + length, "Keyboard %d %d %d ", ctrl, shift, alt);
    for (uint8_t i = 0; i < len; i++) {
        length += sprintf(buffer + length, "%u ", keys[i]);
    }
    for (uint8_t i = 0; i < 6 - len; i++) {
		length += sprintf(buffer + length, "%u ", 0);
	}

    length += sprintf(buffer + length, "\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) buffer, strlen(buffer), 1000);
}

void generate_output_from_state(StateOutput *state) {
    write_mouse(state->mouse.status & (1 << 0), state->mouse.status & (1 << 1), state->mouse.speed_x, state->mouse.speed_y);
    uint16_t keys[6] = {};
    int len = 0;
    if (state->keyboard.forward.pressed) {
        keys[len++] = CODE_W;
    }
    if (state->keyboard.left.pressed) {
        keys[len++] = CODE_A;
    }
    if (state->keyboard.backward.pressed) {
        keys[len++] = CODE_S;
    }
    if (state->keyboard.right.pressed) {
        keys[len++] = CODE_D;
    }
    if (state->keyboard.jump.pressed) {
        keys[len++] = CODE_SPACE;
    }
    if (state->keyboard.reload.pressed) {
		keys[len++] = CODE_R;
	}


//    if (state->keyboard.crouch) {
//        keys[len++] = CODE_LSHIFT;
//    }


    write_keyboard(state->keyboard.crouch.pressed, 0, 0,
                   keys,
                   len);
}
