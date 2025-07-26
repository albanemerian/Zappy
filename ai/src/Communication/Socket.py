##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Socket
##

import socket
from src.Exceptions.Exceptions import (
    SocketException, PlayerDead
)

BUFFER_SIZE = 4096


class Socket:
    def __init__(self, host: str, port: int):
        self._host = host
        self._port = port
        self._address = (host, port)
        self._socket = None

    def connect(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect(self._address)

    def get_fd(self) -> int:
        if self._socket is None:
            raise SocketException("Socket is not connected")
        return self._socket.fileno()

    def send(self, content: str):
        try:
            self._socket.sendall(content.encode("utf-8"))
        except (ConnectionResetError, ConnectionAbortedError, BrokenPipeError) as e:
            raise SocketException(f"Socket connection lost while sending: {e}")
        except socket.error as e:
            raise SocketException(f"Socket send error: {e}")

    def receive(self) -> str:
        try:
            data = self._socket.recv(BUFFER_SIZE)
            if not data:
                raise SocketException("Socket connection closed by the server")
            return data.decode("utf-8")
        except (ConnectionResetError, ConnectionAbortedError, BrokenPipeError) as e:
            raise SocketException(f"Socket connection lost: {e}")
        except socket.error as e:
            raise SocketException(f"Socket error: {e}")

    def close(self):
        if self._socket:
            try:
                self._socket.close()
            except Exception:
                pass
            finally:
                self._socket = None
