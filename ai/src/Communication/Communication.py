##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Communication
##

import select
import threading
import time

from .Socket import Socket
from src.Exceptions.Exceptions import (
    CommunicationHandshakeException,
    CommunicationInvalidResponseException,
    SocketException,
)
from src.Logger.Logger import Logger


class Communication:
    def __init__(self, name: str, host: str, port: int):
        self.requestQueue: list[str] = []
        self.pendingQueue: list[str] = []
        self.responseQueue: list[str] = []

        self.responseBuffer: str = ""

        self.messageQueue: list[tuple[int, str]] = []
        self.playerDead: bool = False
        self.lastInventory: dict[str, int] = {}
        self.lastLook: list[dict[str, int]] = []

        self.name = name
        self.host = host
        self.port = port

        self.socket = Socket(host, port)

        self.mutex = threading.Lock()

        self.logger = Logger()

    def __del__(self):
        try:
            self.stopLoop()
            if self.socket:
                self.socket.close()
        except Exception:
            pass

    def stopLoop(self) -> None:
        with self.mutex:
            self.playerDead = True

    def loop(self) -> None:
        while not self.playerDead:
            try:
                if self.lenRequestQueue() > 0 and self.lenPendingQueue() < 10:
                    with self.mutex:
                        if len(self.requestQueue) > 0:
                            request = self.requestQueue.pop(0)
                            try:
                                self.send(request)
                                self.pendingQueue.append(request)
                            except CommunicationInvalidResponseException:
                                self.requestQueue.insert(0, request)
                self.receive()
            except SocketException:
                with self.mutex:
                    self.playerDead = True
                break
            except CommunicationInvalidResponseException:
                with self.mutex:
                    self.playerDead = True
                break

    def tryGetInventory(self, response: str) -> dict[str, int] | None:
        if not response.startswith("[") or not response.endswith("]"):
            return None

        items = response[1:-1].split(",")
        inventory = dict()
        for item in items:
            if not item:
                continue
            try:
                data = [d.strip() for d in item.strip().split(" ") if d and d.strip()]
                if len(data) != 2:
                    return None
                name, quantity = data
                if not name or not quantity:
                    return None
                inventory[name] = int(quantity)
            except ValueError:
                return None

        return inventory

    def tryGetLook(self, response: str) -> list[dict[str, int]] | None:
        if not response.startswith("[") or not response.endswith("]"):
            return None

        try:
            look = []
            tiles = response[1:-1].split(",")
            if not tiles or len(tiles) == 0:
                return None
            for tile in tiles:
                look.append({})
                if not tile:
                    continue
                items = tile.strip().split(" ")
                items = [d.strip() for d in items if d and d.strip()]
                for item in items:
                    if not item:
                        continue
                    if item not in look[-1]:
                        look[-1][item] = 0
                    look[-1][item] += 1
            return look
        except (IndexError, KeyError, ValueError) as e:
            self.logger.error(f"Error parsing look response '{response}': {e}")
            return None

    def handleResponse(self, response: str) -> str:
        if response.startswith("dead"):
            self.playerDead = True
            return "dead"
        if response.startswith("message "):
            parts = response.split(" ")
            number = int(parts[1].strip(","))
            data = parts[2]
            self.messageQueue.append((number, data))
            return ""
        inventory = self.tryGetInventory(response)
        if inventory is not None:
            self.lastInventory = inventory
            return "inventory"
        look = self.tryGetLook(response)
        if look is not None:
            self.lastLook = look
            return "look"
        return response

    def receiveData(self) -> str:
        try:
            shouldRec = False
            with self.mutex:
                if '\n' not in self.responseBuffer:
                    shouldRec = True
            if shouldRec:
                r = self.socket.receive()
                with self.mutex:
                    self.responseBuffer += r
            with self.mutex:
                firstNewlineIndex = self.responseBuffer.index('\n')
                data = self.responseBuffer[:firstNewlineIndex + 1]
                self.responseBuffer = self.responseBuffer[firstNewlineIndex + 1:]
                return self.handleResponse(data.strip())
        except SocketException:
            raise
        except Exception:
            return ""

    def send(self, msg: str) -> None:
        poller_object = select.poll()
        poller_object.register(self.socket.get_fd(), select.POLLOUT)
        fd_vs_event = poller_object.poll(200)
        fd, event = fd_vs_event[0]
        if event & select.POLLOUT:
            self.socket.send(msg)
        elif event & select.POLLHUP:
            self.logger.error("Server disconnected (POLLHUP)")
            self.playerDead = True
            raise CommunicationInvalidResponseException(
                "Server disconnected while trying to send data"
            )
        elif event & select.POLLERR:
            self.logger.error("Communication error (POLLERR)")
            self.playerDead = True
            raise CommunicationInvalidResponseException(
                "Error in communication with server while sending data"
            )
        else:
            raise CommunicationInvalidResponseException(
                "Socket not ready for sending data"
            )

    def receive(self) -> None:
        try:
            poller_object = select.poll()
            poller_object.register(self.socket.get_fd(), select.POLLIN)
            fd_vs_event = poller_object.poll(200)

            for fd, event in fd_vs_event:
                if event & select.POLLHUP:
                    self.logger.error("Server disconnected (POLLHUP)")
                    with self.mutex:
                        self.playerDead = True
                    return
                if event & select.POLLERR:
                    self.logger.error("Communication error (POLLERR)")
                    with self.mutex:
                        self.playerDead = True
                    raise CommunicationInvalidResponseException(
                        "Error in communication with server"
                    )
                if event & select.POLLIN:
                    data = self.receiveData()
                    if data:
                        self.addResponse(data)
                        with self.mutex:
                            if len(self.pendingQueue) > 0:
                                self.pendingQueue.pop(0)
        except SocketException:
            raise
        except Exception as e:
            self.logger.error(f"Unexpected error in receive: {e}")
            with self.mutex:
                self.playerDead = True

    def getInventory(self) -> dict[str, int]:
        with self.mutex:
            return self.lastInventory

    def getLook(self) -> list[dict[str, int]]:
        with self.mutex:
            return self.lastLook

    def lenMessageQueue(self) -> int:
        with self.mutex:
            return len(self.messageQueue)

    def hasMessages(self) -> bool:
        with self.mutex:
            return len(self.messageQueue) > 0

    def getLastMessage(self) -> tuple[int, str]:
        with self.mutex:
            if len(self.messageQueue) > 0:
                return self.messageQueue.pop(0)
            return (0, "")

    def lenResponseQueue(self) -> int:
        with self.mutex:
            return len(self.responseQueue)

    def hasResponses(self) -> bool:
        with self.mutex:
            return len(self.responseQueue) > 0

    def addResponse(self, response: str) -> None:
        with self.mutex:
            self.responseQueue.append(response)

    def getLastResponse(self) -> str:
        with self.mutex:
            if len(self.responseQueue) > 0:
                return self.responseQueue.pop(0)
            return ""

    def lenPendingQueue(self) -> int:
        with self.mutex:
            return len(self.pendingQueue)

    def hasPendingCommands(self) -> bool:
        with self.mutex:
            return len(self.pendingQueue) > 0

    def lenRequestQueue(self) -> int:
        with self.mutex:
            return len(self.requestQueue)

    def hasRequests(self) -> bool:
        with self.mutex:
            return len(self.requestQueue) > 0

    def playerIsDead(self) -> bool:
        with self.mutex:
            return self.playerDead

    def connectToServer(self):
        self.socket.connect()
        response = self.receiveData()

        if response != "WELCOME":
            raise CommunicationInvalidResponseException(
                f"Invalid response from server handshake: {response}"
            )

        while (True):
            try:
                self.send(f"{self.name}\n")
                break
            except CommunicationInvalidResponseException as e:
                self.logger.error(f"Failed to send name: {e}")
                time.sleep(0.5)
                continue
        response = self.receiveData()

        if response == "ko":
            raise CommunicationHandshakeException(
                f"Handshake failed: server return KO after sending: '{self.name}'"
            )

        slots = 0
        try:
            slots = int(response)
        except ValueError:
            raise CommunicationInvalidResponseException(
                f"Invalid number of slots: {response}"
            )

        response = self.receiveData()

        x = 0
        y = 0
        try:
            x, y = map(int, response.split(" "))
        except ValueError:
            raise CommunicationInvalidResponseException(
                f"Invalid coordinates from server: {response}"
            )

        if slots < 0 or x < 0 or y < 0:
            raise CommunicationHandshakeException(
                f"Invalid handshake values: slots={slots}, x={x}, y={y}"
            )
        return slots, x, y

    def sendCommand(self, message: str) -> None:
        with self.mutex:
            self.requestQueue.append(message)

    def sendForward(self):
        self.sendCommand("Forward\n")

    def sendRight(self):
        self.sendCommand("Right\n")

    def sendLeft(self):
        self.sendCommand("Left\n")

    def sendLook(self) -> None:
        self.sendCommand("Look\n")

    def sendInventory(self) -> None:
        self.sendCommand("Inventory\n")

    def sendBroadcast(self, message: str):
        self.sendCommand(f"Broadcast {message}\n")

    def sendGetConnectNbr(self) -> None:
        self.sendCommand("Connect_nbr\n")

    def sendFork(self):
        self.sendCommand("Fork\n")

    def sendEject(self):
        self.sendCommand("Eject\n")

    def sendTakeObject(self, object_name: str):
        self.sendCommand(f"Take {object_name}\n")

    def sendSetObject(self, object_name: str):
        self.sendCommand(f"Set {object_name}\n")

    def sendIncantation(self):
        self.sendCommand("Incantation\n")
