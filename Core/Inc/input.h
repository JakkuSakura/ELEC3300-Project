#pragma once

#include "jy60.h"
#include "state.h"
#include "stm32f1xx_it.h"

#define BTN_TRIGGER GPIOA, GPIO_PIN_0

static inline GPIO_PinState read_btn_trigger() {
    return HAL_GPIO_ReadPin(BTN_TRIGGER);
}

#define BTN_AIM GPIOC, GPIO_PIN_1

static inline GPIO_PinState read_btn_aim() {
    return HAL_GPIO_ReadPin(BTN_AIM);
}


#define BTN_RELOAD GPIOC, GPIO_PIN_13

// should be using interrupt
static inline GPIO_PinState read_btn_reload() {
    return HAL_GPIO_ReadPin(BTN_RELOAD);
}

#define BTN_CROUCH GPIOC, GPIO_PIN_14

static inline GPIO_PinState read_btn_crouch() {
    return HAL_GPIO_ReadPin(BTN_CROUCH);
}

static inline void read_into_state(State *state) {
    state->input_btn.aim.pressed = read_btn_aim();
    state->input_btn.trigger.pressed = read_btn_trigger();
    state->input_btn.reload.pressed = read_btn_reload();
    state->input_btn.crouch.pressed = read_btn_crouch();

//    // TODO: controller needs ADC
//    state->input_joystick.x = 0;
    state->input_joystick.x = HAL_ADC_GetValue(&hadc1);
    state->input_joystick.y = HAL_ADC_GetValue(&hadc2);
    static uint8_t buffer[64];
    uint16_t len = 0;
    HAL_UARTEx_ReceiveToIdle(&huart2, buffer, 64, &len, 1000);
    if (len > 0) {
        parse_jy60(buffer, len, &state->rotation, &state->angular_velocity);
    }
}
