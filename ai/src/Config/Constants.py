##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Constants
##

USAGE_STRING = "Usage: ./zappy_ai -p port -n name [-h machine]\n" \
               "option\t\tdescription\n" \
               "-p port\t\tport number\n" \
               "-n name\t\tname of the team\n" \
               "-h machine\tname of the machine; localhost by default"

FAILURE = 84
SUCCESS = 0


class Colors:
    BOLD = "\033[1m"
    RED = "\033[1m\033[31m"
    GREEN = "\033[1m\033[32m"
    YELLOW = "\033[1m\033[33m"
    BLUE = "\033[1m\033[34m"
    MAGENTA = "\033[1m\033[35m"
    CYAN = "\033[1m\033[36m"
    WHITE = "\033[1m\033[37m"
    RESET = "\033[0m"
