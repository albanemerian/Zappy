##
## EPITECH PROJECT, 2025
## zappy
## File description:
## test_com
##

import pytest
import sys
import os
from unittest.mock import Mock, patch, MagicMock

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../ai/')))

from src.Communication.Communication import Communication
from src.Communication.Socket import Socket
from src.Exceptions.Exceptions import (
    CommunicationHandshakeException,
    CommunicationInvalidResponseException
)


class TestCommunication:
    ...