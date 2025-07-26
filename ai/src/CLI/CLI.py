##
## EPITECH PROJECT, 2025
## zappy
## File description:
## CLI
##

from src.Exceptions.Exceptions import (
    CLIPortException,
    CLINameException,
    CLIMachineException,
    CLIMissingArgumentException,
    CLIInvalidArgumentException
)


class CLI:
    def __init__(self):
        self.port = None
        self.name = None
        self.machine = "127.0.0.1"

    def parse_args(self, args):
        if len(args) < 5:
            raise CLIInvalidArgumentException("Not enough arguments provided")

        port_found = False
        name_found = False

        i = 1
        while i < len(args):
            if i + 1 >= len(args):
                raise CLIMissingArgumentException(f"Missing value for option: {args[i]}")

            if args[i] == "-p":
                self.port = self.parse_port(args[i + 1])
                port_found = True
                i += 2
            elif args[i] == "-n":
                self.name = self.parse_name(args[i + 1])
                name_found = True
                i += 2
            elif args[i] == "-h":
                self.machine = self.parse_machine(args[i + 1])
                i += 2
            else:
                raise CLIInvalidArgumentException(f"Unknown option: {args[i]}")

        self.validate_config(port_found, name_found)

        return {"port": self.port, "name": self.name, "machine": self.machine}

    def parse_port(self, port_str):
        try:
            port = int(port_str)

            if port <= 0:
                raise CLIPortException("Port must be a positive integer")

            if port > 65535:
                raise CLIPortException("Port must be less than or equal to 65535")

            return port
        except ValueError:
            raise CLIPortException("Port must be a valid number")

    def parse_name(self, name):
        if not name or name.strip() == "":
            raise CLINameException("Team name cannot be empty")
        return name

    def parse_machine(self, machine_str):
        if not machine_str or machine_str.strip() == "":
            raise CLIMachineException("Machine name cannot be empty")

        if machine_str == "localhost":
            return "127.0.0.1"

        try:
            parts = machine_str.split(".")
            if len(parts) != 4:
                raise CLIMachineException("IP address must have exactly 4 segments")

            for part in parts:
                value = int(part)
                if value < 0 or value > 255:
                    raise CLIMachineException("IP address segments must be between 0 and 255")

        except ValueError:
            raise CLIMachineException("Invalid machine name or IP address")

        return machine_str

    def validate_config(self, port_found, name_found):
        if not port_found:
            raise CLIMissingArgumentException("Missing required option: -p port")

        if not name_found:
            raise CLIMissingArgumentException("Missing required option: -n name")
