/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_aiMessage
*/

#include "zappy.h"
#include "game.h"
#include "network.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>


static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Testforward_messages, test_forward_message_north, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = NORTH;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_south, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = SOUTH;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_east, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = EAST;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_west, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = WEST;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_wrapping_north, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 0;  // At top edge
    player.direction = NORTH;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_wrapping_south, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 9;  // At bottom edge
    player.direction = SOUTH;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_wrapping_west, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 0;  // At left edge
    player.posY = 5;
    player.direction = WEST;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_wrapping_east, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 9;  // At right edge
    player.posY = 5;
    player.direction = EAST;
    
    forward_message(&player, &params);
}


Test(Testforward_messages, test_forward_invalid_direction, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = 999;  // Invalid direction
    
    forward_message(&player, &params);
}

Test(TestMoveForward, test_move_forward_invalid_position, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    params.y = 10;
    player.posX = -1;
    player.posY = 5;
    player.direction = NORTH;
    int result = forward_message(&player, &params);
    cr_assert_eq(result, -1, "Expected -1 for invalid position");
}

Test(TestMoveForward_ok, test_move_forward_invalid_position, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    params.x = 10;
    params.y = 10;
    player.posX = 1;
    player.posY = 5;
    player.direction = SOUTH;
    int result = forward_message(&player, &params);
    cr_assert_eq(result, 0, "Expected 0 for invalid position");
}

Test(forward_left, test_forward_left, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = WEST;
    
    forward_message(&player, &params);
}

Test(forward_right, test_forward_right, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = EAST;
    
    forward_message(&player, &params);
}


Test(forward_ok, test_forward_ok, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    params.y = 10;
    player.posX = -5;
    player.posY = 5;
    player.direction = SOUTH;
    
    int result = forward_message(&player, &params);
    cr_assert_eq(result, -1, "Expected -1 for valid position");
}

Test(forward_message, test_forward_message, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    params.y = 10;
    player.posX = -5;
    player.posY = 5;
    player.direction = WEST;
    
    int result = forward_message(&player, &params);
    cr_assert_eq(result, -1, "Expected -1 for valid forward message");
}
