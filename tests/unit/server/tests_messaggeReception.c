/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_messaggeReception
*/


#include "zappy.h"
#include "game.h"
#include "network.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Tests for action_handler.c
Test(MessageReception, test_create_action_request_valid, .init = redirect_all_std)
{
    player_t player = {0};
    char *command = "Forward";
    
    action_request_t *action = create_action_request(command, &player, 20);
    
    cr_assert_not_null(action, "Action should be created successfully");
    cr_assert_str_eq(action->command, "Forward", "Command should be copied correctly");
    cr_assert_eq(action->player, &player, "Player pointer should be set correctly");
    cr_assert_eq(action->priority, PRIORITY_CRITICAL, "Priority should be set from command info");
    cr_assert_null(action->next, "Next pointer should be NULL");
    
    free_action_request(action);
}

Test(MessageReception, test_create_action_request_unknown_command, .init = redirect_all_std)
{
    player_t player = {0};
    char *command = "UnknownCommand";
    
    action_request_t *action = create_action_request(command, &player, 20);
    
    cr_assert_not_null(action, "Action should be created even for unknown command");
    cr_assert_str_eq(action->command, "UnknownCommand", "Command should be copied correctly");

    free_action_request(action);
}

Test(MessageReception, test_init_action_queue, .init = redirect_all_std)
{
    action_queue_t *queue = init_action_queue();
    
    cr_assert_not_null(queue, "Queue should be initialized successfully");
    cr_assert_null(queue->head, "Head should be NULL initially");
    cr_assert_null(queue->tail, "Tail should be NULL initially");
    cr_assert_eq(queue->count, 0, "Count should be 0 initially");
    
    free_action_queue(queue);
}

Test(MessageReception, test_dequeue_highest_priority_action_empty, .init = redirect_all_std)
{
    action_queue_t *queue = init_action_queue();
    
    action_request_t *action = dequeue_highest_priority_action(queue);
    
    cr_assert_null(action, "Should return NULL for empty queue");
    
    free_action_queue(queue);
}

Test(MessageReception, test_dequeue_highest_priority_action_single, .init = redirect_all_std)
{
    action_queue_t *queue = init_action_queue();
    player_t player = {0};
    action_request_t *test_action = create_action_request("Forward", &player, 20);
    
    queue->head = test_action;
    queue->tail = test_action;
    queue->count = 1;
    
    action_request_t *action = dequeue_highest_priority_action(queue);
    
    cr_assert_eq(action, test_action, "Should return the single action");
    cr_assert_null(queue->head, "Head should be NULL after dequeue");
    cr_assert_null(queue->tail, "Tail should be NULL after dequeue");
    cr_assert_eq(queue->count, 0, "Count should be 0 after dequeue");
    cr_assert_null(action->next, "Action next should be NULL");
    
    free_action_request(action);
    free_action_queue(queue);
}

Test(MessageReception, test_insert_action_by_priority, .init = redirect_all_std)
{
    action_queue_t *queue = init_action_queue();
    player_t player = {0};
    action_request_t *action1 = create_action_request("Forward", &player, 20);
    action_request_t *action2 = create_action_request("Forward", &player, 20);
    
    action1->priority = PRIORITY_CRITICAL;
    action2->priority = PRIORITY_CRITICAL;
    
    queue->head = action1;
    queue->tail = action1;
    
    insert_action_by_priority(queue, action2);
    free_action_request(action1);
    free_action_request(action2);
    free(queue);
}

// Tests for loop_thru_command.c
Test(MessageReception, test_find_command_info_valid, .init = redirect_all_std)
{
    const command_info_t *cmd_info = find_command_info("Forward");
    
    cr_assert_not_null(cmd_info, "Should find Forward command");
    cr_assert_str_eq(cmd_info->command, "Forward", "Command name should match");
    cr_assert_eq(cmd_info->base_time, 7, "Base time should be 7");
    cr_assert_eq(cmd_info->priority, PRIORITY_CRITICAL, "Priority should be medium");
}

Test(MessageReception, test_find_command_info_invalid, .init = redirect_all_std)
{
    const command_info_t *cmd_info = find_command_info("InvalidCommand");
    
    cr_assert_null(cmd_info, "Should return NULL for invalid command");
}

// Tests for pollin_handler.c
Test(MessageReception, test_free_action_request_valid, .init = redirect_all_std)
{
    player_t player = {0};
    action_request_t *action = create_action_request("Forward", &player, 20);
    
    // This should not crash
    free_action_request(action);
    cr_assert(true, "free_action_request should handle valid action");
}

Test(MessageReception, test_free_action_request_null, .init = redirect_all_std)
{
    // This should not crash
    free_action_request(NULL);
    cr_assert(true, "free_action_request should handle NULL action");
}

Test(MessageReception, test_smart_poll_players_null_zappy, .init = redirect_all_std)
{
    // This should not crash
    process_player_actions_tick(NULL);
    cr_assert(true, "process_player_actions_tick should handle NULL zappy");
}

Test(MessageReception, test_smart_poll_players_null_game, .init = redirect_all_std)
{
    zappy_t zappy = {0};
    
    // This should not crash
    process_player_actions_tick(&zappy);
    cr_assert(true, "process_player_actions_tick should handle NULL game");
}
