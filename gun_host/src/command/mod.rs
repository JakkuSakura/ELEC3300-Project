use crate::command::keyboard::{handle_command_keyboard, CommandKeyboard, CommandKeyboardState};
use crate::command::mouse::{handle_command_mouse, parse_command_mouse, CommandMouse};
use enigo::Direction;
use eyre::Result;

mod keyboard;
mod mouse;
fn to_direction(press: bool) -> Direction {
    if press {
        Direction::Press
    } else {
        Direction::Release
    }
}
pub enum Command {
    Mouse(CommandMouse),
    Keyboard(CommandKeyboard),
}

pub fn parse_command(s: &str) -> Option<Command> {
    let command = s.split_whitespace().next()?;
    match command {
        "Mouse" => parse_command_mouse(s).map(Command::Mouse),
        "Keyboard" => keyboard::parse_command_keyboard(s).map(Command::Keyboard),
        _ => None,
    }
}
pub struct CommandState {
    device: enigo::Enigo,
    keyboard: CommandKeyboardState,
}
impl CommandState {
    pub fn new() -> Result<Self> {
        let device = enigo::Enigo::new(&enigo::Settings::default())?;

        let this = Self {
            device,
            keyboard: CommandKeyboardState::new(),
        };
        Ok(this)
    }
}
pub fn handle_command(state: &mut CommandState, command: Command) -> Result<()> {
    match command {
        Command::Mouse(cmd) => {
            handle_command_mouse(&mut state.device, cmd)?;
        }
        Command::Keyboard(cmd) => {
            handle_command_keyboard(&mut state.device, &mut state.keyboard, cmd)?;
        }
    }
    Ok(())
}