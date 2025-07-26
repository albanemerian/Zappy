##
## EPITECH PROJECT, 2025
## zappy
## File description:
## main
##

from src.Config.Constants import (
    USAGE_STRING,
    FAILURE,
    SUCCESS
)
from src.CLI.CLI import CLI
from src.Exceptions.Exceptions import CLIParsingException
from src.Player.Player import Player
from sys import argv
from src.Communication.Communication import Communication
from src.App.App import App
from src.Logger.Logger import Logger


def main():
    logger = Logger()

    if len(argv) == 2 and argv[1] == "-help":
        logger.help(USAGE_STRING)
        return SUCCESS

    try:
        cli = CLI()
        config = cli.parse_args(argv)

        logger.success(
            "AI initialized with configuration: "
            f"Port: {config['port']}, "
            f"Team name: {config['name']}, "
            f"Machine: {config['machine']}"
        )

        app = App(config)
        return app.run()

    except CLIParsingException as e:
        logger.error(f"CLI Parsing Error: {e}")
        logger.help(USAGE_STRING)
        return FAILURE

    except KeyboardInterrupt:
        logger.info(f"\nReceived keyboard interrupt, shutting down...")
        return SUCCESS

    except Exception as e:
        logger.error(f"An unexpected error occurred: {e}")
        return FAILURE
