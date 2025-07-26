##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Broadcaster
##

from src.Communication.Communication import Communication
from src.Hash.Hash import Hash


class Broadcaster:
    def __init__(self, com: Communication, team: str) -> None:
        self.com = com
        self.hasher = Hash(team)

    def revealMessage(self, message: str) -> str:
        try:
            message = self.hasher.unHashMessage(message.strip())
        except Exception as e:
            print(f"Error unhashing message: {e}")
            message = ""

        if not message:
            return ""

        return message.strip()

    def broadcastMessage(self, message: str) -> None:
        if not message:
            return

        message = message.strip()
        if not message:
            return

        hashedMessage = ""
        try:
            hashedMessage = self.hasher.hashMessage(message)
        except Exception as e:
            print(f"Error hashing message: {e}")
            return

        if not hashedMessage:
            return

        self.com.sendBroadcast(hashedMessage)
