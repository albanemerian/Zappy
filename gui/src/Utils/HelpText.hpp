/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HelpText
*/

#ifndef HELP_TEXT_HPP_
#define HELP_TEXT_HPP_

namespace zappy::constants {

    inline const char *HELP_TITLE =
        "HELP";

    inline const char *HELP_SECTION_1 =
        "Game Overview";

    inline const char *HELP_SECTION_1_CONTENT =
        "Zappy is a game where AI-controlled players compete to collect resources\n"
        "and level up on a dynamically changing map. The GUI allows you to visualize\n"
        "the game state, players, and resources in real-time.";

    inline const char *HELP_SECTION_2 =
        "Controls";

    inline const char *HELP_SECTION_2_CONTENT =
        "Camera Movement:\n"
        "  - Arrow keys or ZQSD: Move camera\n"
        "  - Controller: Use left stick to move camera\n"
        "  - Right mouse button + drag: Rotate camera\n\n"
        "Interface:\n"
        "  - Click on players to see their stats\n"
        "  - Click on tiles to see their stats\n"
        "  - Use the RESET CAMERA button to return to default view\n"
        "  - Use the Settings button to adjust game settings";

    inline const char *HELP_SECTION_3 =
        "Teams and Players";

    inline const char *HELP_SECTION_3_CONTENT =
        "The left panel shows all teams and their player IDs.\n"
        "Players have different levels based on collected resources.\n"
        "The team that first gets a player to level 8 wins the game.";

    inline const char *HELP_SECTION_4 =
        "Resources";

    inline const char *HELP_SECTION_4_CONTENT =
        "Resources on the map are represented by different colored objects.\n"
        "Players collect these resources to perform rituals and level up.";

    inline const char *HELP_SECTION_5 =
        "Levels";

    inline const char *HELP_SECTION_6 =
        "Items";

}  // namespace zappy::constants

#endif /* !HELP_TEXT_HPP_ */
