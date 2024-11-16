import serial
import serial.tools.list_ports
import time
import pyautogui

pyautogui.
class Command:
    pass


class CommandMouse(Command):
    def __init__(self, left: bool, right: bool, x: int, y: int):
        self.left = left
        self.right = right
        self.x = x
        self.y = y

    def __repr__(self):
        return f"CommandMouse(left={self.left}, right={self.right}, x={self.x}, y={self.y})"


def parse_command(s: str) -> Command:
    parts = s.split()
    command = parts[0] if parts else None
    if command == "Mouse":
        return parse_command_mouse(s)


def parse_command_mouse(s: str) -> Command:
    parts = s.split()
    if len(parts) < 5 or parts[0] != "Mouse":
        return None

    try:
        left = int(parts[1]) > 0
        right = int(parts[2]) > 0
        x = int(parts[3])
        y = int(parts[4])
    except (ValueError, IndexError):
        return None

    return CommandMouse(left, right, x, y)


def handle_command(cmd: Command):
    if isinstance(cmd, CommandMouse):
        handle_command_mouse(cmd)


def handle_command_mouse(cmd: CommandMouse):
    """
    Handle mouse commands using pyautogui.

    :param cmd: CommandMouse object containing mouse command details.
    """

    pyautogui.move(cmd.x, cmd.y)

    # Perform mouse clicks based on the command
    if cmd.left:
        pyautogui.mouseDown(button=pyautogui.LEFT)
    if cmd.right:
        pyautogui.mouseDown(button=pyautogui.RIGHT)


def main():
    print("Available ports:")
    ports = serial.tools.list_ports.comports()

    for i, p in enumerate(ports):
        print(f"{i}. {p.device}")

    selected_index = int(input("Select a port: ").strip())
    selected_port = ports[selected_index].device
    print(f"Selected port: {selected_port}")

    # Open the serial port
    with serial.Serial(selected_port, 115200, timeout=10) as ser:

        while True:
            line = ser.readline()  # Read until newline
            if not line:
                break

            s = line.decode("utf-8", errors="ignore").strip()
            print(f"> {s}")

            command = parse_command(s)
            if not command:
                continue
            print("command", command)
            handle_command(command)

    print("Done")


if __name__ == "__main__":
    main()
