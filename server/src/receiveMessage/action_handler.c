/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** action_handler
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This function creates the "queue" of action for the current player */
action_request_t *create_action_request(char *command, player_t *player,
    int frequency)
{
    action_request_t *action = malloc(sizeof(action_request_t));
    const command_info_t *cmd_info = NULL;

    if (!action)
        return NULL;
    action->command = strdup(command);
    action->timestamp = time(NULL);
    action->player = player;
    action->next = NULL;
    cmd_info = find_command_info(command);
    if (cmd_info) {
        action->priority = cmd_info->priority;
        action->time_limit = cmd_info->base_time / frequency;
    } else {
        action->priority = PRIORITY_LOW;
        action->time_limit = 7 / frequency;
    }
    return action;
}

/* This function remove the action with the highest prioroty */
action_request_t *dequeue_highest_priority_action(action_queue_t *queue)
{
    action_request_t *action = NULL;

    if (!queue->head)
        return NULL;
    action = queue->head;
    queue->head = action->next;
    if (!queue->head)
        queue->tail = NULL;
    queue->count--;
    action->next = NULL;
    return action;
}

/* This function initialize the queue of action for a player */
action_queue_t *init_action_queue(void)
{
    action_queue_t *queue = malloc(sizeof(action_queue_t));

    if (!queue)
        return NULL;
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
    return queue;
}

/* This function adds the requested action in the queue */
void insert_action_by_priority(action_queue_t *queue, action_request_t *action)
{
    action->next = NULL;
    if (!queue->head) {
        queue->head = action;
        queue->tail = action;
    } else {
        queue->tail->next = action;
        queue->tail = action;
    }
    queue->count++;
}
