##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Logger
##

from src.Config.Constants import Colors


class Logger:
    def error(self, message: str) -> None:
        print(f"{Colors.RED}{message}{Colors.RESET}")

    def info(self, message: str) -> None:
        print(f"{Colors.YELLOW}{message}{Colors.RESET}")

    def help(self, message: str) -> None:
        print(f"{Colors.CYAN}{message}{Colors.RESET}")

    def debug(self, message: str) -> None:
        print(message)

    def success(self, message: str) -> None:
        print(f"{Colors.GREEN}{message}{Colors.RESET}")

    def display(self, message: str) -> None:
        print(message)
