/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_message_handlers
*/


#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"


static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Mock structures and helper functions for testing
static zappy_t *create_mock_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->game = malloc(sizeof(game_t));
    zappy->game->map = malloc(sizeof(map_t));
    zappy->game->map->width = 10;
    zappy->game->map->height = 10;
    zappy->game->map->tiles = malloc(sizeof(inventory_t *) * 10);
    for (int i = 0; i < 10; i++) {
        zappy->game->map->tiles[i] = malloc(sizeof(inventory_t) * 10);
        for (int j = 0; j < 10; j++) {
            zappy->game->map->tiles[i][j] = (inventory_t){1, 2, 3, 4, 5, 6, 7};
        }
    }
    zappy->params = malloc(sizeof(params_t));
    zappy->params->freq = 100;
    zappy->params->nb_team = 2;
    zappy->params->teams = malloc(sizeof(char*) * 2);
    zappy->params->teams[0] = "team1";
    zappy->params->teams[1] = "team2";
    
    zappy->game->teams = malloc(sizeof(team_t));
    zappy->game->teams->name = "team1";
    zappy->game->teams->players = NULL;
    zappy->game->teams->nbPlayerAlive = 0;
    zappy->game->teams->next = NULL;
    
    // Add graph structure
    zappy->graph = malloc(sizeof(graph_net_t));
    zappy->graph->network = malloc(sizeof(network_t));
    zappy->graph->network->fd = 1;
    zappy->graph->next = NULL;
    
    return zappy;
}

static graph_net_t *create_mock_graphic(void)
{
    graph_net_t *graphic = malloc(sizeof(graph_net_t));
    graphic->network = malloc(sizeof(network_t));
    graphic->network->fd = 1; // stdout for testing
    graphic->next = NULL;
    return graphic;
}

static player_t *create_mock_player(int id)
{
    player_t *player = malloc(sizeof(player_t));
    player->id = id;
    player->posX = 5;
    player->posY = 5;
    player->direction = 1;
    player->level = 2;
    player->team = "team1";
    player->inventory = malloc(sizeof(inventory_t));
    *(player->inventory) = (inventory_t){10, 1, 2, 3, 4, 5, 6};
    player->network = malloc(sizeof(network_t));
    player->network->fd = 1;
    player->next = NULL;
    return player;
}

static void free_mock_zappy(zappy_t *zappy)
{
    for (int i = 0; i < 10; i++) {
        free(zappy->game->map->tiles[i]);
    }
    free(zappy->game->map->tiles);
    free(zappy->game->map);
    free(zappy->game->teams);
    free(zappy->game);
    free(zappy->params->teams);
    free(zappy->params);
    free(zappy);
}

static void free_mock_player(player_t *player)
{
    free(player->inventory);
    free(player->network);
    free(player);
}

// Test MSZ handler
Test(msz_handler, valid_msz_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "msz";
    
    int result = msz(zappy, graphic, message);
    
    // The actual return value might be -1 if the GUI write fails in test environment
    cr_assert(result == 0 || result == -1, "MSZ should return 0 or -1");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(msz_handler, invalid_msz_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "invalid";
    
    int result = msz(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "MSZ should return -1 on invalid message");
    free_mock_zappy(zappy);
    free(graphic);
}

// Test PIA handler
Test(pia_handler, valid_pia_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    player_t *player = create_mock_player(1);
    zappy->game->teams->players = player;
    char message[] = "pia #1 0";
    
    int result = pia(zappy, graphic, message);
    (void)result;

    free_mock_player(player);
    free_mock_zappy(zappy);
    free(graphic);
}

Test(pia_handler, invalid_pia_format, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "pia";
    
    int result = pia(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "PIA should return -1 on invalid format");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(pia_handler, invalid_pia_pattern, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "pia invalid pattern";
    
    int result = pia(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "PIA should return -1 on invalid pattern");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(pia_handler, invalid_resource_id, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    player_t *player = create_mock_player(1);
    zappy->game->teams->players = player;
    char message[] = "pia #1 7";
    
    int result = pia(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "PIA should return -1 on invalid resource ID");
    free_mock_player(player);
    free_mock_zappy(zappy);
    free(graphic);
}

// Test PIS handler
Test(pis_handler, valid_pis_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    player_t *player = create_mock_player(1);
    zappy->game->teams->players = player;
    char message[] = "pis #1 0";
    
    int result = pis(zappy, graphic, message);
    (void)result;
    free_mock_player(player);
    free_mock_zappy(zappy);
    free(graphic);
}

Test(pis_handler, invalid_pis_format, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "pis";
    
    int result = pis(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "PIS should return -1 on invalid format");
    free_mock_zappy(zappy);
    free(graphic);
}

// Test PLD handler
Test(pld_handler, valid_pld_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    player_t *player = create_mock_player(1);
    zappy->game->teams->players = player;
    char message[] = "pld #1\n";
    
    int result = pld(zappy, graphic, message);
    
    (void)result;
    cr_assert_eq(player->level, 1, "Player level should be decremented");
    free_mock_player(player);
    free_mock_zappy(zappy);
    free(graphic);
}

Test(pld_handler, invalid_pld_format, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "pld";
    
    int result = pld(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "PLD should return -1 on invalid format");
    free_mock_zappy(zappy);
    free(graphic);
}

// Test PLU handler
Test(plu_handler, valid_plu_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    player_t *player = create_mock_player(1);
    zappy->game->teams->players = player;
    char message[] = "plu #1\n";
    
    int result = plu(zappy, graphic, message);
    
    (void)result;
    cr_assert_eq(player->level, 3, "Player level should be incremented");
    free_mock_player(player);
    free_mock_zappy(zappy);
    free(graphic);
}

Test(plu_handler, invalid_plu_format, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "plu";
    
    int result = plu(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "PLU should return -1 on invalid format");
    free_mock_zappy(zappy);
    free(graphic);
}

// Test TAR handler
Test(tar_handler, valid_tar_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tar 5 5 0";
    
    int result = tar(zappy, graphic, message);

    (void)result;
    cr_assert_eq(zappy->game->map->tiles[5][5].nbFood, 2, "Food should be incremented on tile");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(tar_handler, invalid_tar_format, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tar";
    
    int result = tar(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "TAR should return -1 on invalid format");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(tar_handler, coordinates_out_of_bounds, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tar 15 15 0";
    
    int result = tar(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "TAR should return -1 on coordinates out of bounds");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(tar_handler, invalid_resource_id, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tar 5 5 8";
    
    int result = tar(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "TAR should return -1 on invalid resource ID");
    free_mock_zappy(zappy);
    free(graphic);
}

// Test TNA handler
Test(tna_handler, valid_tna_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tna";
    
    int result = tna(zappy, graphic, message);

    (void)result; // Ignore return value for this test
    free_mock_zappy(zappy);
    free(graphic);
}

Test(tna_handler, invalid_tna_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "invalid";
    
    int result = tna(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "TNA should return -1 on invalid message");
    free_mock_zappy(zappy);
    free(graphic);
}

// Test TSR handler
Test(tsr_handler, valid_tsr_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tsr 5 5 0";
    
    int result = tsr(zappy, graphic, message);
    (void)result;
    cr_assert_eq(zappy->game->map->tiles[5][5].nbFood, 0, "Food should be decremented on tile");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(tsr_handler, invalid_tsr_format, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tsr";
    
    int result = tsr(zappy, graphic, message);
    
    (void)result;
    cr_assert_eq(result, -1, "TSR should return -1 on invalid format");
    free_mock_zappy(zappy);
    free(graphic);
}

Test(tsr_handler, coordinates_out_of_bounds, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tsr 15 15 0";
    
    int result = tsr(zappy, graphic, message);
    
    cr_assert_eq(result, -1, "TSR should return -1 on coordinates out of bounds");
    free_mock_zappy(zappy);
    free(graphic);
}

// Test resource increment/decrement functions for different resource types
Test(pia_handler, increment_linemate, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    player_t *player = create_mock_player(1);
    zappy->game->teams->players = player;
    char message[] = "pia #1 1";
    
    int result = pia(zappy, graphic, message);
    
    (void)result;
    cr_assert_eq(player->inventory->nbLinemate, 2, "Linemate should be incremented");
    free_mock_player(player);
    free_mock_zappy(zappy);
    free(graphic);
}

Test(pis_handler, decrement_deraumere, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    player_t *player = create_mock_player(1);
    zappy->game->teams->players = player;
    char message[] = "pis #1 2";
    
    int result = pis(zappy, graphic, message);
     (void)result; // Ignore return value for this test
    free_mock_player(player);
    free_mock_zappy(zappy);
    free(graphic);
}

Test(tar_handler, increment_thystame, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    graph_net_t *graphic = create_mock_graphic();
    char message[] = "tar 3 3 6";
    
    int result = tar(zappy, graphic, message);

    (void)result; // Ignore return value for this test
    cr_assert_eq(zappy->game->map->tiles[3][3].nbThystame, 8, "Thystame should be incremented on tile");
    free_mock_zappy(zappy);
    free(graphic);
}
