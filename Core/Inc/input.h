#pragma once

#include "jy60.h"
#include "state.h"
#include "stm32f1xx_it.h"

#define BTN_TRIGGER GPIOA, GPIO_PIN_6
//#define BTN_AIM GPIOC, GPIO_PIN_1
#define BTN_SWITCH GPIOC, GPIO_PIN_1
#define BTN_JUMP GPIOC, GPIO_PIN_1
#define BTN_RELOAD GPIOC, GPIO_PIN_13
#define BTN_CONTINOUS GPIOC, GPIO_PIN_13
#define BTN_CROUCH GPIOC, GPIO_PIN_14

static inline void read_buttons(StateInputButton *state) {
//    state->aim.pressed = read_btn_aim();
    state->trigger.pressed = HAL_GPIO_ReadPin(BTN_TRIGGER);
    state->jump.pressed = HAL_GPIO_ReadPin(BTN_JUMP);
    state->switch_.pressed = HAL_GPIO_ReadPin(BTN_SWITCH);
    state->continuous.pressed = HAL_GPIO_ReadPin(BTN_CONTINOUS);
    state->reload.pressed = HAL_GPIO_ReadPin(BTN_RELOAD);
    state->crouch.pressed = HAL_GPIO_ReadPin(BTN_CROUCH);
}
// here X and Y are the joystick's x and y
// ADC 1
#define CTL_MOVE_X GPIOA, GPIO_PIN_5
// ADC 2
#define CTL_MOVE_Y GPIOA, GPIO_PIN_4

static inline void init_joystick_offset(StateInputJoystick *state) {
    state->y_offset = (int32_t)HAL_ADC_GetValue(&hadc1);
    state->x_offset = (int32_t)HAL_ADC_GetValue(&hadc2);

}

static inline void read_joystick(StateInputJoystick *state) {
    state->y = (int32_t)HAL_ADC_GetValue(&hadc1) - state->y_offset;
    state->x = (int32_t)HAL_ADC_GetValue(&hadc2) - state->x_offset;

}

static inline void read_into_state(State *state) {
    read_buttons(&state->input_btn);
    read_joystick(&state->input_joystick);

    static uint8_t buffer[64];
    uint16_t len = 0;
    HAL_UARTEx_ReceiveToIdle(&huart3, buffer, sizeof(buffer), &len, 10);
    // string in C ends with \0
    if (len > 1) {
        parse_jy60(buffer, len, &state->rotation, &state->angular_velocity);
    }
}
