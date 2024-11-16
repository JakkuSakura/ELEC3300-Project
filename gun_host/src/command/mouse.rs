use crate::command::to_direction;
use enigo::{Button, Coordinate, Enigo, Mouse};
use eyre::Result;
use std::str::FromStr;

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
    if cmd != "Mouse" {
        return None;
    }
    let left = i8::from_str(parts.next()?).ok()? > 0;
    let right = i8::from_str(parts.next()?).ok()? > 0;
    let x = i8::from_str(parts.next()?).ok()?;
    let y = i8::from_str(parts.next()?).ok()?;
    Some(CommandMouse { left, right, x, y })
}

pub fn handle_command_mouse(mouse: &mut Enigo, command: CommandMouse) -> Result<()> {
    mouse.button(Button::Left, to_direction(command.left))?;
    mouse.button(Button::Right, to_direction(command.right))?;
    mouse.move_mouse(command.x as i32, command.y as i32, Coordinate::Rel)?;
    Ok(())
}
