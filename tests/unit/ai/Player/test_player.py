#
# EPITECH PROJECT, 2025
# zappy
# File description:
# test_player.py


import pytest
import sys
import os
from unittest.mock import patch, MagicMock, Mock
from time import sleep
from src.Player.Player import Player
from src.Hash.Hash import Hash
from src.Exceptions.Exceptions import PlayerDead

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../../../ai'))


class TestPlayer:
    ...

