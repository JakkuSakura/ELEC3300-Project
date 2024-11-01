#pragma once

#include <stdint.h>
#include <memory.h>

typedef struct {
    char name[16];
    uint16_t pressed;
    uint16_t reacted;
} StateButton;

typedef struct {
    StateButton trigger;
    StateButton aim;
    StateButton reload;
    StateButton continuous;
} StateInputButton;

inline void init_state_input_button(StateInputButton *input) {
    bzero(input, sizeof(StateInputButton));
    strcpy(input->trigger.name, "trigger");
    strcpy(input->aim.name, "aim");
    strcpy(input->reload.name, "reload");
    strcpy(input->continuous.name, "continuous");
}

// joystick
const int8_t JOYSTICK_THRESHOLD = 10;
typedef struct {
    int8_t x;
    int8_t y;
} StateInputJoystick;

inline void init_state_input_joystick(StateInputJoystick *joystick) {
    bzero(joystick, sizeof(StateInputJoystick));
}
// https://deskthority.net/wiki/Scancode

// reload
const int32_t CODE_R = 0x15;
// WASD
const int32_t CODE_W = 0x1A;
const int32_t CODE_A = 0x04;
const int32_t CODE_S = 0x16;
const int32_t CODE_D = 0x07;

// jump
const int32_t CODE_SPACE = 0x2C;
// crouch
const int32_t CODE_LSHIFT = 0x12;
typedef struct {
    uint32_t code;
    uint8_t pressed;
} OutputKey;


typedef struct {
    OutputKey reload;
    OutputKey forward;
    OutputKey backward;
    OutputKey left;
    OutputKey right;
    OutputKey jump;
    OutputKey crouch;
} OutputKeyboard;

inline void init_output_keyboard(OutputKeyboard *output) {
    bzero(output, sizeof(OutputKeyboard));
    output->reload.code = CODE_R;
    output->forward.code = CODE_W;
    output->backward.code = CODE_S;
    output->left.code = CODE_A;
    output->right.code = CODE_D;
    output->jump.code = CODE_SPACE;
    output->crouch.code = CODE_LSHIFT;
}

// fire
const uint8_t MOUSE_LEFT = 0x01;
// aim
const uint8_t MOUSE_RIGHT = 0x02;
const int8_t MOUSE_SPEED = 16;
typedef struct {
    uint8_t status;
    // left negative, right positive
    int8_t speed_x;
    // up negative, down positive
    int8_t speed_y;

} OutputMouse;

inline void init_output_mouse(OutputMouse *output) {
    bzero(output, sizeof(OutputMouse));
}

typedef struct {
    OutputKeyboard keyboard;
    OutputMouse mouse;
} StateOutput;

inline void init_state_output(StateOutput *output) {
    init_output_keyboard(&output->keyboard);
    init_output_mouse(&output->mouse);
}

const float ACCELERATION_VELOCITY_THRESHOLD = 0.1f;
typedef struct {
    float x;
    float y;
    float z;
} StateAngularVelocity;

inline void init_state_rotate(StateAngularVelocity *rotate) {
    bzero(rotate, sizeof(StateAngularVelocity));
}
const float ROTATION_THRESHOLD = 0.1f;
typedef struct {
    float x;
    float y;
    float z;
} StateRotation;

inline void init_state_rotation(StateRotation *rotation) {
    bzero(rotation, sizeof(StateRotation));
}

typedef struct {
    StateInputButton input_btn;
    StateInputJoystick input_joystick;
    StateAngularVelocity angular_velocity;
    StateRotation rotation;
    StateOutput output;
} State;

inline void init_state(State *state) {
    init_state_input_button(&state->input_btn);
    init_state_input_joystick(&state->input_joystick);
    init_state_rotate(&state->angular_velocity);
    init_state_rotation(&state->rotation);
    init_state_output(&state->output);
}

inline void apply_state(State *state) {
    init_state_output(&state->output);
    // mouse
    if (state->input_btn.trigger.pressed) {
        state->output.mouse.status |= MOUSE_LEFT;
    }
    if (state->input_btn.aim.pressed) {
        state->output.mouse.status |= MOUSE_RIGHT;
    }
    // keyboard
    if (state->input_btn.reload.pressed) {
        state->output.keyboard.reload.pressed = 1;
    }
    if (state->input_btn.trigger.pressed) {
        state->output.keyboard.forward.pressed = 1;
    }
    if (state->input_joystick.x > JOYSTICK_THRESHOLD) {
        state->output.keyboard.forward.pressed = 1;
    } else if (state->input_joystick.x < -JOYSTICK_THRESHOLD) {
        state->output.keyboard.backward.pressed = 1;
    }
    if (state->input_joystick.y > JOYSTICK_THRESHOLD) {
        state->output.keyboard.right.pressed = 1;
    } else if (state->input_joystick.y < -JOYSTICK_THRESHOLD) {
        state->output.keyboard.left.pressed = 1;
    }

    // movement is handled by angular velocity

    if (state->input_btn.continuous.pressed) {
        if (state->rotation.x > ROTATION_THRESHOLD) {
            state->output.mouse.speed_y = MOUSE_SPEED;
        } else if (state->rotation.x < -ROTATION_THRESHOLD) {
            state->output.mouse.speed_y = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_y = 0;
        }
        if (state->rotation.y > ROTATION_THRESHOLD) {
            state->output.mouse.speed_x = MOUSE_SPEED;
        } else if (state->rotation.y < -ROTATION_THRESHOLD) {
            state->output.mouse.speed_x = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_x = 0;
        }

    } else {
        if (state->angular_velocity.x > ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_x = MOUSE_SPEED;
        } else if (state->angular_velocity.x < -ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_x = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_x = 0;
        }

        if (state->angular_velocity.y > ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_y = MOUSE_SPEED;
        } else if (state->angular_velocity.y < -ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_y = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_y = 0;
        }
    }

}