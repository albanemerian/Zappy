/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Constants
*/

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

    inline const float PLAYER_SCALE = 0.005f;
    inline const float EGG_SCALE = 1.0f;
    inline const float FOOD_SCALE = 0.005f;
    inline const float FOOD_FLOAT_AMPLITUDE = 0.05f;
    inline const float FOOD_FLOAT_SPEED = 1.5f;
    inline const char *CUSTOM_FONT_PATH = "gui/assets/fonts/fall.ttf";
    inline const float ROCK_SCALE = 0.2f;
    inline const float LINEMATE_SCALE = 0.2f;    // soccerball
    inline const float DERAUMERE_SCALE = 0.15f;  // beachball
    inline const float SIBUR_SCALE = 0.15f;      // basketball
    inline const float MENDIANE_SCALE = 0.18f;   // bowlingball
    inline const float PHIRAS_SCALE = 0.1f;      // eightball
    inline const float THYSTAME_SCALE = 0.1f;    // tennisball

#include <string>
#include <vector>
#include "HelpText.hpp"
#include "../IDisplay.hpp"

namespace zappy::constants {

    inline const char *USAGE_STRING = "USAGE: ./zappy_gui -p port -h machine\n"
                               "option\t\tdescription\n"
                               "-p port\t\tport number\n"
                               "-h machine\thostname of the server";

    inline const int FAILURE_EXIT_CODE = 84;
    inline const int SUCCESS_EXIT_CODE = 0;
};

namespace colors {

    inline const char *T_BOLD = "\033[1m";
    inline const char *T_RED = "\033[1m\033[31m";
    inline const char *T_GREEN = "\033[1m\033[32m";
    inline const char *T_YELLOW = "\033[1m\033[33m";
    inline const char *T_BLUE = "\033[1m\033[34m";
    inline const char *T_MAGENTA = "\033[1m\033[35m";
    inline const char *T_CYAN = "\033[1m\033[36m";
    inline const char *T_WHITE = "\033[1m\033[37m";
    inline const char *RESET = "\033[0m";

};

namespace zappy::structs {

    struct Config {
        int port;
        std::string hostname;
    };

    struct Tile {
        int x;
        int y;
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;

        Tile(int _x = 0, int _y = 0, int _food = 0, int _linemate = 0,
             int _deraumere = 0, int _sibur = 0, int _mendiane = 0,
             int _phiras = 0, int _thystame = 0)
            : x(_x), y(_y), food(_food), linemate(_linemate),
              deraumere(_deraumere), sibur(_sibur),
              mendiane(_mendiane), phiras(_phiras), thystame(_thystame) {}
    };

    struct Inventory {
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;

        Inventory(int _food = 0, int _linemate = 0, int _deraumere = 0,
                  int _sibur = 0, int _mendiane = 0, int _phiras = 0,
                  int _thystame = 0)
            : food(_food), linemate(_linemate), deraumere(_deraumere),
              sibur(_sibur), mendiane(_mendiane), phiras(_phiras),
              thystame(_thystame) {}
    };
    struct Player {
        int number;
        int x;
        int y;
        int orientation;
        int level;
        std::string teamName;
        struct Inventory inventory;

        Player(int _number = 0, int _x = 0, int _y = 0, int _orientation = 0,
               int _level = 1, const std::string &_teamName = "",
               struct Inventory _inventory = Inventory())
            : number(_number), x(_x), y(_y), orientation(_orientation),
              level(_level), teamName(_teamName), inventory(_inventory) {}
    };

    struct Incantation {
        int x;
        int y;
        int level;
        std::vector<int> players;

        Incantation(int _x = 0, int _y = 0, int _level = 1,
                    const std::vector<int> &_players = {})
            : x(_x), y(_y), level(_level), players(_players) {}
    };

    struct Egg {
        int eggNumber;
        int playerNumber;
        int x;
        int y;
        bool hatched;
        std::string teamName;

        Egg(int _eggNumber = 0, int _playerNumber = 0, int _x = 0, int _y = 0,
            bool _hatched = false, const std::string &_teamName = "")
            : eggNumber(_eggNumber), playerNumber(_playerNumber), x(_x), y(_y),
              hatched(_hatched), teamName(_teamName) {}
    };
};

namespace zappy::gui {

    inline const char *WINDOW_TITLE = "Zappy GUI";
    inline const unsigned int FPS = 60;
    inline const char *CUSTOM_FONT_PATH = "gui/assets/fonts/fall.ttf";

    inline const char *SPLASH_TITLE = "ZAPPY";
    inline const char *SPLASH_SUBTITLE = "Advanced 3D Game Visualizer";

    inline const float CAMERA_SENSITIVITY = 0.001f;
    inline const float GAMEPAD_STICK_SENSITIVITY = 3.0f;
    inline const float GAMEPAD_DEADZONE = 0.2f;
    inline const float POSITION_MULTIPLIER = 2.2f;

    inline const float FOG_DISTANCE_MAX = 60.0f;
    inline const float DURATION_DAYNIGHT_CYCLE = 120.0f;

    inline const float EGG_SCALE = 1.0f;
    inline const float FOOD_SCALE = 0.005f;
    inline const float FOOD_FLOAT_AMPLITUDE = 0.05f;
    inline const float FOOD_FLOAT_SPEED = 1.0f;

    inline const float LINEMATE_SCALE = 0.2f;    // soccerball
    inline const float DERAUMERE_SCALE = 0.15f;  // beachball
    inline const float SIBUR_SCALE = 0.15f;      // basketball
    inline const float MENDIANE_SCALE = 0.18f;   // bowlingball
    inline const float PHIRAS_SCALE = 0.1f;      // eightball
    inline const float THYSTAME_SCALE = 0.1f;    // tennisball

    inline const float PLAYER_ROTATION_SPEED = 720.0f;
    inline const float ROTATION_INTERPOLATION_THRESHOLD = 1.0f;

    inline const float PLAYER_MOVEMENT_SPEED = 8.0f;
    inline const float MOVEMENT_INTERPOLATION_THRESHOLD = 0.05f;

    enum class CameraMode {
        FREE = 0,
        TARGETED = 1,
        PLAYER = 2,
        NB_MODES = 3,
    };


    struct PlayerModelInfo {
        std::string name;
        std::string modelPath;
        Vector3f center;
        Vector3f scale;
        float rotation;
    };

    inline const std::vector<PlayerModelInfo> PLAYER_MODELS_INFO = {
        {"playerLvl1", "gui/assets/models/playerLvl1.glb",
            {0.0f, -0.0f, 0.0f}, {0.005f, 0.005f, 0.005f}, 0.0f},
        {"playerLvl2", "gui/assets/models/playerLvl2.glb",
            {0.0f, -0.5f, 0.0f}, {0.25f, 0.25f, 0.25f}, 0.0f},
        {"playerLvl3", "gui/assets/models/playerLvl3.glb",
            {0.0f, 20.0f, 0.0f}, {0.0045f, 0.0045f, 0.0045f}, 0.0f},
        {"playerLvl4", "gui/assets/models/playerLvl4.glb",
            {0.0f, 0.0025f, 0.0f}, {40.0f, 40.0f, 40.0f}, -90.0f},
        {"playerLvl5", "gui/assets/models/playerLvl5.glb",
            {8.0f, -1.8f, 0.0f}, {0.2f, 0.2f, 0.2f}, 0.0f},
        {"playerLvl6", "gui/assets/models/playerLvl6.glb",
            {0.0f, 20.0f, 0.0f}, {0.009f, 0.009f, 0.009f}, 0.0f},
        {"playerLvl7", "gui/assets/models/playerLvl7.glb",
            {0.0f, 0.4f, 0.0f}, {0.25f, 0.25f, 0.25f}, 0.0f},
        {"playerLvl8", "gui/assets/models/playerLvl8.glb",
            {0.0f, 1.0f, 0.0f}, {0.085, 0.085f, 0.085f}, 0.0f}
    };
}

#endif /* !CONSTANTS_HPP_ */
