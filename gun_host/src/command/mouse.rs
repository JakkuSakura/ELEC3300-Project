use crate::command::to_direction;
use enigo::{Button, Coordinate, Enigo, Mouse};
use eyre::Result;
use std::str::FromStr;

pub struct CommandMouseState {
    pub left: bool,
    pub right: bool,
}
impl CommandMouseState {
    pub fn new() -> Self {
        CommandMouseState {
            left: false,
            right: false,
        }
    }
}
#[derive(Debug)]
pub struct CommandMouse {
    pub left: bool,
    pub right: bool,
    pub x: i8,
    pub y: i8,
}
// Mouse <left> <right> <x> <y>
// all are integers
pub fn parse_command_mouse(s: &str) -> Option<CommandMouse> {
    let mut parts = s.split_whitespace();
    let cmd = parts.next()?;
    if cmd != "M" {
        return None;
    }
    let left = i8::from_str(parts.next()?).ok()? > 0;
    let right = i8::from_str(parts.next()?).ok()? > 0;
    let x = i8::from_str(parts.next()?).ok()?;
    let y = i8::from_str(parts.next()?).ok()?;
    Some(CommandMouse { left, right, x, y })
}

pub fn handle_command_mouse(
    mouse: &mut Enigo,
    mouse_state: &mut CommandMouseState,
    command: CommandMouse,
) -> Result<()> {
    if mouse_state.left != command.left {
        mouse.button(Button::Left, to_direction(command.left))?;
        mouse_state.left = command.left;
    }
    if mouse_state.right != command.right {
        mouse.button(Button::Right, to_direction(command.right))?;
        mouse_state.right = command.right;
    }
    if command.x != 0 || command.y != 0 {
        mouse.move_mouse(command.x as i32, command.y as i32, Coordinate::Rel)?;
    }
    Ok(())
}
