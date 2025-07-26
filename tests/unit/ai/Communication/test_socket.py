##
## EPITECH PROJECT, 2025
## zappy
## File description:
## test_socket
##

import pytest
import sys
import os
import socket
import threading
import time
from unittest.mock import Mock, patch, MagicMock

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../ai/')))

from src.Communication.Socket import Socket
from src.Exceptions.Exceptions import SocketException


class TestSocket:
    def test_socket_init(self):
        """Test socket initialization"""
        sock = Socket("localhost", 4242)

        assert sock._host == "localhost"
        assert sock._port == 4242
        assert sock._address == ("localhost", 4242)
        assert sock._socket is None

    @patch('socket.socket')
    def test_socket_connect_success(self, mock_socket):
        """Test successful socket connection"""
        mock_socket_instance = Mock()
        mock_socket.return_value = mock_socket_instance

        sock = Socket("localhost", 4242)
        sock.connect()

        mock_socket.assert_called_once_with(socket.AF_INET, socket.SOCK_STREAM)
        mock_socket_instance.connect.assert_called_once_with(("localhost", 4242))
        assert sock._socket == mock_socket_instance

    @patch('socket.socket')
    def test_socket_connect_failure(self, mock_socket):
        """Test socket connection failure"""
        mock_socket_instance = Mock()
        mock_socket_instance.connect.side_effect = ConnectionRefusedError("Connection refused")
        mock_socket.return_value = mock_socket_instance

        sock = Socket("localhost", 4242)

        with pytest.raises(ConnectionRefusedError):
            sock.connect()

    @patch('socket.socket')
    def test_socket_send_success(self, mock_socket):
        """Test successful message sending"""
        mock_socket_instance = Mock()
        mock_socket.return_value = mock_socket_instance

        sock = Socket("localhost", 4242)
        sock.connect()
        sock.send("test message")

        mock_socket_instance.sendall.assert_called_once_with(b"test message")

    @patch('socket.socket')
    def test_socket_send_unicode(self, mock_socket):
        """Test sending unicode messages"""
        mock_socket_instance = Mock()
        mock_socket.return_value = mock_socket_instance

        sock = Socket("localhost", 4242)
        sock.connect()
        sock.send("tëst mëssagë")

        mock_socket_instance.sendall.assert_called_once_with("tëst mëssagë".encode("utf-8"))

    @patch('socket.socket')
    def test_socket_receive_connection_closed(self, mock_socket):
        """Test handling closed connection during receive"""
        mock_socket_instance = Mock()
        mock_socket_instance.recv.return_value = b""
        mock_socket.return_value = mock_socket_instance

        sock = Socket("localhost", 4242)
        sock.connect()

        with pytest.raises(SocketException) as excinfo:
            sock.receive()

        assert "Socket connection closed by the server" in str(excinfo.value)

    @patch('socket.socket')
    def test_socket_receive_unicode(self, mock_socket):
        """Test receiving unicode messages"""
        mock_socket_instance = Mock()
        mock_socket_instance.recv.return_value = "tëst mëssagë\n".encode("utf-8")
        mock_socket.return_value = mock_socket_instance

        sock = Socket("localhost", 4242)
        sock.connect()
        result = sock.receive()

        assert result == "tëst mëssagë\n"

    @patch('socket.socket')
    def test_socket_close(self, mock_socket):
        """Test socket close"""
        mock_socket_instance = Mock()
        mock_socket.return_value = mock_socket_instance

        sock = Socket("localhost", 4242)
        sock.connect()
        sock.close()

        mock_socket_instance.close.assert_called_once()

    def test_socket_different_hosts_and_ports(self):
        """Test socket creation with different hosts and ports"""
        sock1 = Socket("127.0.0.1", 8080)
        sock2 = Socket("192.168.1.1", 9090)
        sock3 = Socket("example.com", 443)

        assert sock1._address == ("127.0.0.1", 8080)
        assert sock2._address == ("192.168.1.1", 9090)
        assert sock3._address == ("example.com", 443)
