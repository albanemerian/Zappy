##
## EPITECH PROJECT, 2025
## zappy
## File description:
## App
##

import os
import signal
import time
from src.Config.Constants import (
    FAILURE,
    SUCCESS,
)
from src.Player.Player import Player
from src.Exceptions.Exceptions import (
    CommunicationException,
    SocketException
)
from src.Logger.Logger import Logger


class App:
    def __init__(self, config: dict[str]):
        self.port = config["port"]
        self.name = config["name"]
        self.ip = config["machine"]
        self.childs: list[int] = []
        self.running = True
        self.is_main_process = True
        self.logger = Logger()
        self.mainPlayer: Player = None

        if self.is_main_process:
            signal.signal(signal.SIGINT, self._signal_handler)
            signal.signal(signal.SIGTERM, self._signal_handler)

    def _signal_handler(self, signum, frame):
        if self.is_main_process:
            self.logger.info(f"Received signal {signum}, shutting down AI team {self.name}...")
            self._cleanup_children()
            self.running = False
            if self.mainPlayer is not None:
                self.mainPlayer.communication.stopLoop()

    def _wait_for_children(self):
        if not self.is_main_process:
            return

        if len(self.childs) > 0:
            self.logger.info(f"Waiting for {len(self.childs)} AI child processes to finish...")

        for pid in self.childs:
            try:
                os.waitpid(pid, 0)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        num_children = len(self.childs)
        self.childs.clear()
        if num_children > 0:
            self.logger.success(f"All AI processes finished.")

    def _cleanup_children(self):
        if not self.is_main_process:
            return

        if len(self.childs) > 0:
            self.logger.info(f"Terminating {len(self.childs)} AI child processes...")

        for pid in self.childs:
            try:
                os.kill(pid, signal.SIGTERM)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        time.sleep(0.5)

        force_killed = []
        for pid in self.childs:
            try:
                os.kill(pid, 0)
                os.kill(pid, signal.SIGKILL)
                force_killed.append(pid)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        for pid in self.childs:
            try:
                os.waitpid(pid, 0)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        num_children = len(self.childs)
        self.childs.clear()
        if force_killed:
            self.logger.info(f"Force killed {len(force_killed)} AI child processes")
        if num_children > 0:
            self.logger.success(f"All AI processes terminated.")

    def create_new_player(self) -> int:
        pid: int = os.fork()
        if pid < 0:
            return -1
        if pid == 0:
            self.is_main_process = False
            try:
                p = Player(self.name, self.ip, self.port)
                p.is_child_process = True
                result = p.start()
                exit(result)
            except Exception:
                exit(FAILURE)
        return pid

    def run(self):
        self.logger.success(f"Starting Zappy AI for team: {self.name}...")
        self.mainPlayer = Player(self.name, self.ip, self.port)
        slots = 0

        try:
            slots, x, y = self.mainPlayer.communication.connectToServer()
            self.mainPlayer.setMapSize(x, y)
            self.mainPlayer.setNbSlots(slots + 1)
        except (CommunicationException, SocketException) as e:
            self.logger.error(f"Failed to connect to server: {e}")
            return FAILURE

        for _ in range(slots):
            if not self.running:
                break
            child_pid = self.create_new_player()
            if child_pid > 0:
                self.childs.append(child_pid)

        try:
            self.mainPlayer.startComThread()
            while self.running:
                try:
                    self.mainPlayer.loop()
                    break
                except KeyboardInterrupt:
                    break
        except (CommunicationException, SocketException):
            self.logger.error(f"Server connection lost for team {self.name}")
            if self.running:
                self._cleanup_children()
            return FAILURE
        except KeyboardInterrupt:
            self.logger.info(f"Interrupted - shutting down team {self.name}")
            if self.running:
                self._cleanup_children()
            return SUCCESS
        except Exception as e:
            self.logger.error(f"Unexpected error in main player: {e}")
            if self.running:
                self._cleanup_children()
            return FAILURE
        finally:
            if self.is_main_process:
                self._wait_for_children()
                self.logger.info(f"AI team {self.name} finished")

        return SUCCESS
