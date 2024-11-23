mod command;

use crate::command::{handle_command, parse_command, CommandState};
use eyre::{Context, Result};
use std::io::{BufRead, BufReader};
use std::time::Duration;

fn execute_task(port_name: &str) -> Result<()> {
    println!("Starting task {}...", port_name);
    let port = serialport::new(port_name, 115_200)
        .timeout(Duration::from_secs(10))
        .open()
        .context("Failed to open port")?;
    let mut reader = BufReader::new(port);
    let mut buf = Vec::with_capacity(1024);
    let mut state = CommandState::new()?;
    loop {
        buf.clear();
        let n = reader.read_until(b'\n', &mut buf)?;
        if n == 0 {
            break;
        }
        if buf.trim_ascii().is_empty() {
            continue;
        }

        let s = String::from_utf8_lossy(&buf);
        // println!("> {}", s);
        let Some(command) = parse_command(&s) else {
            println!("Invalid command: {:?}", s);
            continue;
        };
        handle_command(&mut state, command)?;
    }
    println!("finished");
    Ok(())
}
fn main() -> Result<()> {
    println!("Available ports:");
    let ports = serialport::available_ports().expect("No ports found!");
    for (i, p) in ports.iter().enumerate() {
        println!("{}. {}", i, p.port_name);
    }
    println!("Select a port:");
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    let int: usize = input.trim().parse().unwrap();
    println!("Selected port: {}", ports[int].port_name);
    let port = &ports[int];
    loop {
        if let Err(err) = execute_task(port.port_name.as_str()) {
            println!("Failed to execute task: {}", err);
        }
    }
}
