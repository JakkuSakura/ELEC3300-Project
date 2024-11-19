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
    StateButton jump;
    StateButton switch_;
    StateButton reload;
    StateButton continuous;
    StateButton crouch;
} StateInputButton;

static inline void init_state_input_button(StateInputButton *input) {
    bzero(input, sizeof(StateInputButton));
    strcpy(input->trigger.name, "trigger");
    strcpy(input->aim.name, "aim");
    strcpy(input->reload.name, "reload");
    strcpy(input->jump.name, "jump");
    strcpy(input->switch_.name, "switch");
    strcpy(input->continuous.name, "continuous");
    strcpy(input->crouch.name, "crouch");
}

// joystick
static const int16_t JOYSTICK_THRESHOLD = 500;
typedef struct {
    int32_t x;
    int32_t y;
} StateInputJoystick;

static inline void init_state_input_joystick(StateInputJoystick *joystick) {
    bzero(joystick, sizeof(StateInputJoystick));
}
// https://deskthority.net/wiki/Scancode

// reload
static const int32_t CODE_R = 0x15;
// WASD
static const int32_t CODE_W = 0x1A;
static const int32_t CODE_A = 0x04;
static const int32_t CODE_S = 0x16;
static const int32_t CODE_D = 0x07;

// jump
static const int32_t CODE_SPACE = 0x2C;
// crouch does it has a key? what about modifier
static const int32_t CODE_LSHIFT = 0x12;
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

static inline void init_output_keyboard(OutputKeyboard *output) {
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
static const uint8_t MOUSE_LEFT = 0x01;
// aim
static const uint8_t MOUSE_RIGHT = 0x02;
static const int8_t MOUSE_SPEED = 16;
typedef struct {
    uint8_t status;
    // left negative, right positive
    int8_t speed_x;
    // up negative, down positive
    int8_t speed_y;

} OutputMouse;

static inline void init_output_mouse(OutputMouse *output) {
    bzero(output, sizeof(OutputMouse));
}

typedef struct {
    OutputKeyboard keyboard;
    OutputMouse mouse;
} StateOutput;

static inline void init_state_output(StateOutput *output) {
    init_output_keyboard(&output->keyboard);
    init_output_mouse(&output->mouse);
}

static const float ACCELERATION_VELOCITY_THRESHOLD = 100.0f;
typedef struct {
    float roll;
    float pitch;
    float yaw;
} StateAngularVelocity;

static inline void init_state_acc(StateAngularVelocity *rotate) {
    bzero(rotate, sizeof(StateAngularVelocity));
}

static const float ROTATION_THRESHOLD = 5.0f;
typedef struct {
    float roll;
    float pitch;
    float yaw;
    float temp;
} StateRotation;

static inline void init_state_rotation(StateRotation *rotation) {
    bzero(rotation, sizeof(StateRotation));
}

typedef struct {
    StateInputButton input_btn;
    StateInputJoystick input_joystick;
    StateInputJoystick input_joystick_offset;
    StateAngularVelocity angular_velocity;
    StateRotation rotation;
    StateRotation rotation_offset;
    StateOutput output;
} State;

static inline void init_state(State *state) {
    init_state_input_button(&state->input_btn);
    init_state_input_joystick(&state->input_joystick);
    init_state_input_joystick(&state->input_joystick_offset);
    init_state_acc(&state->angular_velocity);
    init_state_rotation(&state->rotation);
    init_state_output(&state->output);
}

static inline void update_state(State *state) {
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
    if (state->input_btn.jump.pressed) {
		state->output.keyboard.jump.pressed = 1;
	}

    if (state->input_joystick.y > JOYSTICK_THRESHOLD) {
        state->output.keyboard.forward.pressed = 1;
    } else if (state->input_joystick.y < -JOYSTICK_THRESHOLD) {
        state->output.keyboard.backward.pressed = 1;
    }
    if (state->input_joystick.x > JOYSTICK_THRESHOLD) {
        state->output.keyboard.right.pressed = 1;
    } else if (state->input_joystick.x < -JOYSTICK_THRESHOLD) {
        state->output.keyboard.left.pressed = 1;
    }

    // movement is handled by angular velocity
    if (state->input_btn.continuous.pressed) {
        state->input_btn.continuous.reacted = 1;
        state->rotation_offset = state->rotation;
    }
    if (1 || state->input_btn.continuous.pressed) {
//        if (!state->input_btn.continuous.reacted) {
//            state->input_btn.continuous.reacted = 1;
//            state->rotation_offset = state->rotation;
//        }
        float roll_offset = state->rotation.roll;
        if (roll_offset > ROTATION_THRESHOLD) {
            state->output.mouse.speed_x = MOUSE_SPEED;
        } else if (roll_offset < -ROTATION_THRESHOLD) {
            state->output.mouse.speed_x = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_x = 0;
        }
        float pitch_offset = state->rotation.pitch;
        if (pitch_offset > ROTATION_THRESHOLD) {
            state->output.mouse.speed_y = MOUSE_SPEED;
        } else if (pitch_offset < -ROTATION_THRESHOLD) {
            state->output.mouse.speed_y = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_y = 0;
        }

    } else {
        state->input_btn.continuous.reacted = 0;
        if (state->angular_velocity.roll > ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_x = MOUSE_SPEED;
        } else if (state->angular_velocity.roll < -ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_x = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_x = 0;
        }

        if (state->angular_velocity.pitch > ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_y = MOUSE_SPEED;
        } else if (state->angular_velocity.pitch < -ACCELERATION_VELOCITY_THRESHOLD) {
            state->output.mouse.speed_y = (int8_t) -MOUSE_SPEED;
        } else {
            state->output.mouse.speed_y = 0;
        }
    }

}
