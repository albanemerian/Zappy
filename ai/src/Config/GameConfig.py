##
## EPITECH PROJECT, 2025
## zappy
## File description:
## GameConfig
##

MAX_LEVEL = 8

ELEVATION_COST = 300

FOOD_VALUE = 126

LVL_UPGRADES = {
    1: {
        "players": 1,
        "stones": {
            "linemate": 1,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
    },
    2: {
        "players": 2,
        "stones": {
            "linemate": 1,
            "deraumere": 1,
            "sibur": 1,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
    },
    3: {
        "players": 2,
        "stones": {
            "linemate": 2,
            "deraumere": 0,
            "sibur": 1,
            "mendiane": 0,
            "phiras": 2,
            "thystame": 0
        }
    },
    4: {
        "players": 4,
        "stones": {
            "linemate": 1,
            "deraumere": 1,
            "sibur": 2,
            "mendiane": 0,
            "phiras": 1,
            "thystame": 0
        }
    },
    5: {
        "players": 4,
        "stones": {
            "linemate": 1,
            "deraumere": 2,
            "sibur": 1,
            "mendiane": 3,
            "phiras": 0,
            "thystame": 0
        }
    },
    6: {
        "players": 6,
        "stones": {
            "linemate": 1,
            "deraumere": 2,
            "sibur": 3,
            "mendiane": 0,
            "phiras": 1,
            "thystame": 0
        }
    },
    7: {
        "players": 6,
        "stones": {
            "linemate": 2,
            "deraumere": 2,
            "sibur": 2,
            "mendiane": 2,
            "phiras": 2,
            "thystame": 1
        }
    },
}

TOTAL_NEEDED_STONES = {
    "linemate": 9,
    "deraumere": 8,
    "sibur": 10,
    "mendiane": 5,
    "phiras": 6,
    "thystame": 1
}
