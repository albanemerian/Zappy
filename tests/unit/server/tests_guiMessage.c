/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_guiMessage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Mock functions
static char *last_written_message = NULL;
static int write_message_called = 0;

int int_str_len(int num)
{
    if (num == 0) return 1;
    int len = 0;
    if (num < 0) {
        len = 1;
        num = -num;
    }
    while (num > 0) {
        len++;
        num /= 10;
    }
    return len;
}

// Test fixtures
static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->graph = malloc(sizeof(graph_net_t));
    zappy->game = malloc(sizeof(game_t));
    zappy->game->map = malloc(sizeof(map_t));
    
    zappy->params->freq = 100;
    zappy->params->is_debug = true;
    zappy->graph->network = malloc(sizeof(network_t));
    zappy->graph->network->fd = 42;
    zappy->game->map->currentEggs = NULL;
    
    return zappy;
}

static player_t *create_test_player(void)
{
    player_t *player = malloc(sizeof(player_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = 123;
    player->posX = 5;
    player->posY = 10;
    player->level = 2;
    player->direction = 1;
    player->team = strdup("team1");
    
    player->inventory->nbFood = 10;
    player->inventory->nbLinemate = 1;
    player->inventory->nbDeraumere = 2;
    player->inventory->nbSibur = 3;
    player->inventory->nbMendiane = 4;
    player->inventory->nbPhiras = 5;
    player->inventory->nbThystame = 6;
    
    return player;
}

static egg_t *create_test_egg(void)
{
    egg_t *egg = malloc(sizeof(egg_t));
    egg->id = 456;
    egg->posX = 7;
    egg->posY = 8;
    egg->idLayer = 789;
    egg->teamName = strdup("team2");
    egg->next = NULL;
    
    return egg;
}

static void cleanup_test_data(zappy_t *zappy, player_t *player, egg_t *egg)
{
    if (zappy) {
        free(zappy->params);
        free(zappy->graph);
        free(zappy->game->map);
        free(zappy->game);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->inventory);
        free(player);
    }
    if (egg) {
        free(egg->teamName);
        free(egg);
    }
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
    write_message_called = 0;
}

// Tests for gui_send_time.c
Test(gui_send_time, send_time_message_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    send_time_message(zappy);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "sgt 100\n");
    
    cleanup_test_data(zappy, NULL, NULL);
}

Test(gui_send_time, send_updated_time_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    send_updated_time(zappy, 50);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "sst 50\n");
    
    cleanup_test_data(zappy, NULL, NULL);
}

// Tests for gui_send_player_info.c
Test(gui_send_player_info, send_player_connect_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_player_connect(zappy, player);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "pnw #123 5 10 2 1 team1\n");
    
    cleanup_test_data(zappy, player, NULL);
}

Test(gui_send_player_info, send_player_pos_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_player_pos(zappy, player);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "ppo #123 5 10 1\n");
    
    cleanup_test_data(zappy, player, NULL);
}

Test(gui_send_player_info, send_player_level_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_player_level(zappy, player);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "plv #123 2\n");
    
    cleanup_test_data(zappy, player, NULL);
}

// Tests for gui_send_rest_info_player.c
Test(gui_send_rest_info_player, send_player_inventory_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_player_inventory(zappy, player);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "pin #123 5 10 10 1 2 3 4 5 6\n");
    
    cleanup_test_data(zappy, player, NULL);
}

Test(gui_send_rest_info_player, send_player_expelled_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_player_expelled(zappy, player);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "pex #123\n");
    
    cleanup_test_data(zappy, player, NULL);
}

// Tests for gui_send_ressource.c
Test(gui_send_ressource, send_ressource_droped_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_ressource_droped(zappy, player, 3);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "pdr #123 3\n");
    
    cleanup_test_data(zappy, player, NULL);
}

Test(gui_send_ressource, send_ressource_collected_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_ressource_collected(zappy, player, 2);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "pgt #123 2\n");
    
    cleanup_test_data(zappy, player, NULL);
}

// Tests for gui_send_eggs.c
Test(gui_send_eggs, send_egg_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    egg_t *egg = create_test_egg();
    
    send_egg(zappy, egg);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "enw #456 #789 7 8 team2\n");
    
    cleanup_test_data(zappy, NULL, egg);
}

Test(gui_send_eggs, send_player_laying_egg_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    
    send_player_laying_egg(zappy, player);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "pfk #123\n");
    
    cleanup_test_data(zappy, player, NULL);
}

Test(gui_send_eggs, send_entire_egg_list_empty, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    send_entire_egg_list(zappy);
    
    // cr_assert_eq(write_message_called, 0);
    
    cleanup_test_data(zappy, NULL, NULL);
}

Test(gui_send_eggs, send_entire_egg_list_with_eggs, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    egg_t *egg1 = create_test_egg();
    egg_t *egg2 = create_test_egg();
    egg2->id = 999;
    egg1->next = egg2;
    zappy->game->map->currentEggs = egg1;
    
    send_entire_egg_list(zappy);
    
    // cr_assert_eq(write_message_called, 2);
    
    free(egg2->teamName);
    free(egg2);
    cleanup_test_data(zappy, NULL, egg1);
}

// Tests for gui_send_egg_connect.c
Test(gui_send_egg_connect, send_egg_connect_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    egg_t *egg = create_test_egg();
    
    send_egg_connect(zappy, egg);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "ebo #456\n");
    
    cleanup_test_data(zappy, NULL, egg);
}

// Tests for gui_miscellaneous_send.c
Test(gui_miscellaneous_send, send_end_game_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    send_end_game(zappy, "winners");
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "seg winners\n");
    
    cleanup_test_data(zappy, NULL, NULL);
}

Test(gui_miscellaneous_send, send_str_message_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    send_str_message(zappy, "hello");
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "smg hello\n");
    
    cleanup_test_data(zappy, NULL, NULL);
}

Test(gui_miscellaneous_send, send_unknown_command_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    send_unknown_command(zappy);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "suc\n");
    
    cleanup_test_data(zappy, NULL, NULL);
}

Test(gui_miscellaneous_send, send_command_parameter_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    send_command_parameter(zappy);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "sbp\n");
    
    cleanup_test_data(zappy, NULL, NULL);
}

// Test debug mode off
Test(gui_send_time, send_time_message_debug_off, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    zappy->params->is_debug = false;
    
    send_time_message(zappy);
    
    // cr_assert_eq(write_message_called, 1);
    // cr_assert_str_eq(last_written_message, "sgt 100\n");
    
    cleanup_test_data(zappy, NULL, NULL);
}


Test(gui_send_broadcast, send_broadcast_message_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();

    send_broadcast_to_player(zappy, player, "Hello, world!");
    cleanup_test_data(zappy, NULL, NULL);
}

Test(gui_send_broadcast, send_broadcast_message_debug_off, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    team_t *team = malloc(sizeof(team_t));
    team->name = strdup("team1");
    player_t *player = create_test_player();

    team->players = player;
    send_broadcast_to_all(zappy, "Hello, world!");
    cleanup_test_data(zappy, NULL, NULL);
}


Test(gui_send_death_player, send_player_death_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();

    send_player_death(zappy, player);
    cleanup_test_data(zappy, player, NULL);
}

Test(gui_egg_death, send_egg_death_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    egg_t *egg = create_test_egg();

    send_egg_death(zappy, egg);
    cleanup_test_data(zappy, NULL, egg);
}


Test(gui_send_map_data, send_map_data_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    zappy->params->x = 10;
    zappy->params->y = 10;
    zappy->game->map->width = 10;
    zappy->game->map->height = 10;

    init_game(zappy);
    send_entrie_map(zappy);
    send_map_size(zappy);
    cleanup_test_data(zappy, NULL, NULL);
}

Test(gui_send_team_name, send_team_name_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    team_t *team = malloc(sizeof(team_t));
    team->name = strdup("team1");

    zappy->game->teams = team;
    send_team_name(zappy);
    free(team->name);
    free(team);
    cleanup_test_data(zappy, NULL, NULL);
}
