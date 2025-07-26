/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** Errors.c tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <unistd.h>

#include "network.h"

#include "zappy.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

/* Server :: Errors :: Helper */

Test(helper, helper_valid, .init = redirect_all_std) {
    cr_assert_eq(helper(), 84, "Expected helper function to return 84.");
}

/* Server :: Errors :: Error Message */

Test(errors, error_message_output, .init = redirect_all_std)
{
    error_message("Test error message");

}

Test(errors, write_message_valid_fd, .init = redirect_all_std)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->network->fd = 1; // Valid file descriptor for testing
    player->network->writingBuffer = malloc(sizeof(buffer_t));
    player->network->writingBuffer = create_buffer('\n');

    write_in_buffer(player->network->writingBuffer, "Test message");
    int result = write_message(player->network);

    cr_assert_geq(result, 0);
}

Test(errors, write_message_invalid_fd, .init = redirect_all_std)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->network->fd = 1; // Valid file descriptor for testing
    player->network->writingBuffer = malloc(sizeof(buffer_t));
    player->network->writingBuffer = create_buffer('\n');

    write_in_buffer(player->network->writingBuffer, "Test message");
    int result = write_message(player->network);
    (void)result;
}

Test(errors, helper_function, .init = redirect_all_std)
{
    int result = helper();
    
    cr_assert_eq(result, 84);
}
