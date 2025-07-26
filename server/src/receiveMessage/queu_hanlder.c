/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** queu_hanlder
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

/* This sends the end message "ok" or "ko" and sets the cooldown of player */
void execute_action(player_t *player, action_request_t *action, zappy_t *zappy)
{
    const command_info_t *cmd_info = find_command_info(action->command);
    int result = 0;

    if (!cmd_info) {
        write_in_buffer(player->network->writingBuffer, "ko\n");
        write_message(player->network);
        return;
    }
    player->time_action = action->time_limit;
    gettimeofday(&player->last_action_time, NULL);
    if (cmd_info->base_time >= 0)
        player->is_busy = true;
    result = cmd_info->handler(player, action->command, zappy);
    if (result == 0) {
        write_in_buffer(player->network->writingBuffer, "ok\n");
        write_message(player->network);
    } else if (result == -1) {
        write_in_buffer(player->network->writingBuffer, "ko\n");
        write_message(player->network);
    }
}

/* This functions queue the actions and "lock" it tosafely acces memory */
void queue_action(player_t *player, char *command, zappy_t *zappy)
{
    action_request_t *action = NULL;

    (void)zappy;
    if (!player->pending_actions || player->pending_actions->count >= 10)
        return;
    action = create_action_request(command, player, zappy->params->freq);
    if (!action)
        return;
    if (!player->pending_actions->head ||
        action->priority < player->pending_actions->head->priority) {
        action->next = player->pending_actions->head;
        player->pending_actions->head = action;
        if (!player->pending_actions->tail)
            player->pending_actions->tail = action;
    } else
        insert_action_by_priority(player->pending_actions, action);
}

/* This function free's the queue */
void free_action_queue(action_queue_t *queue)
{
    action_request_t *current = NULL;
    action_request_t *next = NULL;

    if (!queue)
        return;
    current = queue->head;
    while (current) {
        next = current->next;
        free_action_request(current);
        current = next;
    }
    free(queue);
}
