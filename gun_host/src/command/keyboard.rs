use crate::command::to_direction;
use enigo::Keyboard;
use eyre::ContextCompat;
use std::str::FromStr;

pub struct CommandKeyboard {
    pub ctrl: bool,
    pub alt: bool,
    pub shift: bool,
    pub key: [u32; 6],
}
pub fn parse_command_keyboard(s: &str) -> Option<CommandKeyboard> {
    let mut parts = s.split_whitespace();
    let cmd = parts.next()?;
    if cmd != "Keyboard" {
        return None;
    }
    let ctrl = i8::from_str(parts.next()?).ok()? > 0;
    let alt = i8::from_str(parts.next()?).ok()? > 0;
    let shift = i8::from_str(parts.next()?).ok()? > 0;
    let mut key = [0; 6];
    for i in 0..6 {
        key[i] = parts.next()?.parse().ok()?;
    }
    Some(CommandKeyboard {
        ctrl,
        alt,
        shift,
        key,
    })
}

// reload
pub const CODE_R: u32 = 0x15;
// WASD
pub const CODE_W: u32 = 0x1A;
pub const CODE_A: u32 = 0x04;
pub const CODE_S: u32 = 0x16;
pub const CODE_D: u32 = 0x07;
// jump
pub const CODE_SPACE: u32 = 0x2C;
// crouch
pub const CODE_LSHIFT: u32 = 0x12;
fn map_scancode_to_key(scan_code: u32) -> Option<enigo::Key> {
    if true {
        Some(enigo::Key::Other(scan_code))
    } else {
        match scan_code {
            CODE_R => Some(enigo::Key::Unicode('r')),
            CODE_W => Some(enigo::Key::Unicode('w')),
            CODE_A => Some(enigo::Key::Unicode('a')),
            CODE_S => Some(enigo::Key::Unicode('s')),
            CODE_D => Some(enigo::Key::Unicode('d')),
            CODE_SPACE => Some(enigo::Key::Space),
            CODE_LSHIFT => Some(enigo::Key::Shift),
            _ => None,
        }
    }
}
pub struct CommandKeyboardState {
    len: usize,
    pressed: [u32; 6],
}
impl CommandKeyboardState {
    pub fn new() -> Self {
        Self {
            len: 0,
            pressed: [0; 6],
        }
    }
    pub fn insert(&mut self, key: u32) {
        if self.len < 6 {
            self.pressed[self.len] = key;
            self.len += 1;
        }
    }
    pub fn contains(&self, key: &u32) -> bool {
        self.pressed[..self.len].iter().any(|&x| x == *key)
    }
}
pub fn handle_command_keyboard(
    keyboard: &mut enigo::Enigo,
    state: &mut CommandKeyboardState,
    command: CommandKeyboard,
) -> eyre::Result<()> {
    if command.ctrl {
        keyboard.key(enigo::Key::Control, to_direction(command.ctrl))?;
    }
    if command.alt {
        keyboard.key(enigo::Key::Alt, to_direction(command.alt))?;
    }
    if command.shift {
        keyboard.key(enigo::Key::Shift, to_direction(command.shift))?;
    }
    let mut new_pressed = CommandKeyboardState::new();
    for key in command.key {
        if key == 0 {
            continue;
        }

        keyboard.key(
            map_scancode_to_key(key).context("Invalid key")?,
            enigo::Direction::Press,
        )?;
        new_pressed.insert(key);
    }
    for &key in state.pressed.iter() {
        if !new_pressed.contains(&key) {
            keyboard.key(
                map_scancode_to_key(key).context("Invalid key")?,
                enigo::Direction::Release,
            )?;
        }
    }
    *state = new_pressed;

    Ok(())
}
