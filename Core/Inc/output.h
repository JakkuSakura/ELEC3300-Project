#pragma once

#include <stdio.h>
#include "usart.h"
#include "state.h"

void write_mouse(OutputMouse *mouse) {
    static char buffer[64];
    int length = sprintf(buffer, "M %d %d %d %d                         \r\n", mouse->left, mouse->right,
                         mouse->speed_x,
                         mouse->speed_y);
    HAL_UART_Transmit(&huart1, (uint8_t *) buffer, length, 1000);
}

void write_keyboard(
        OutputKeyboard *keyboard
) {
    static char buffer[128];
    int length = 0;

    int ctrl = keyboard->crouch.pressed;
    int shift = 0;
    int alt = 0;
    length += sprintf(buffer + length, "K %d %d %d ", ctrl, shift, alt);


#define PRESS(key, code) if (keyboard->key.pressed) { length += sprintf(buffer + length, "%u ", code); }
    PRESS(forward, CODE_W)
    PRESS(left, CODE_A)
    PRESS(backward, CODE_S)
    PRESS(right, CODE_D)
    PRESS(jump, CODE_SPACE)
    PRESS(reload, CODE_R)
#undef PRESS


    length += sprintf(buffer + length, "\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) buffer, length, 1000);
}

void generate_output_from_state(StateOutput *state) {
    write_mouse(&state->mouse);
    if (memcmp(&state->keyboard, &state->keyboard_prev, sizeof(OutputKeyboard)) != 0) {
        write_keyboard(&state->keyboard);
        state->keyboard_prev = state->keyboard;
    }
}
