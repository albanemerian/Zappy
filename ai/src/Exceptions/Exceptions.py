##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Exceptions
##


class CLIParsingException(Exception):

    def __init__(self, message: str):
        super().__init__(message)


class CLIPortException(CLIParsingException):

    def __init__(self, message: str):
        super().__init__(f"Port Error: {message}")


class CLINameException(CLIParsingException):

    def __init__(self, message: str):
        super().__init__(f"Name Error: {message}")


class CLIMachineException(CLIParsingException):

    def __init__(self, message: str):
        super().__init__(f"Machine Error: {message}")


class CLIMissingArgumentException(CLIParsingException):

    def __init__(self, message: str):
        super().__init__(f"Missing Argument: {message}")


class CLIInvalidArgumentException(CLIParsingException):

    def __init__(self, message: str):
        super().__init__(f"Invalid Argument: {message}")


class SocketException(Exception):

    def __init__(self, message: str):
        super().__init__(f"Socket Error: {message}")


class CommunicationException(Exception):

    def __init__(self, message: str):
        super().__init__(message)


class CommunicationHandshakeException(CommunicationException):

    def __init__(self, message: str):
        super().__init__(f"Handshake Error: {message}")


class CommunicationInvalidResponseException(CommunicationException):

    def __init__(self, message: str):
        super().__init__(f"Invalid response from server: {message}")


class PlayerDead(CommunicationException):

    def __init__(self):
        super().__init__(f"Player is dead")
