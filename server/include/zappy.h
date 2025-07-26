/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Zappy header
*/

#include <stdbool.h>
#include <poll.h>
#include "game.h"
#include "my.h"

#ifndef ZAPPY_H_
    #define ZAPPY_H_

/* items handler */
typedef struct {
    char *name;
    void (*add_func)(inventory_t *);
} item_handler_t;


/* Cli parameter of the server */
typedef struct params_s {
    int port;
    int x;
    int y;
    int nb_team;
    char **teams;
    int nb_client;
    int freq;
    bool is_debug;
} params_t;

/* Structure to handle the network side of the gui*/
typedef struct graph_net_s {
    network_t *network;
    bool mapSent;
    struct graph_net_s *next;
} graph_net_t;

/* Unified polling structure for all clients */
typedef struct unified_poll_s {
    struct pollfd *fds;
    int count;
    int capacity;
} unified_poll_t;

typedef struct zappy_s {
    server_t *network;
    game_t *game;
    graph_net_t *graph;
    params_t *params;
    unified_poll_t *unified_poll;
} zappy_t;

typedef struct command_pf_s {
    char const *flag;
    bool (*checker)(const char *, const char *, params_t *);
} command_pf_t;

typedef struct {
    char *command;
    float base_time;
    action_priority_t priority;
    int (*handler)(player_t *, char *, zappy_t *);
} command_info_t;

typedef struct graphic_pf_s {
    char *command;
    int (*handler)(zappy_t *zappy, graph_net_t *graphic, char *message);
} graphic_pf_t;

/* messages.c */
int helper(void);
void error_message(const char *message);
void valid_message(char const *message);
int return_error(char const *message);
void other_message(char const *message);

/* checkers.c */
bool check_port(char const *flag, char const *value, params_t *params);
bool check_width(char const *flag, char const *value, params_t *params);
bool check_height(char const *flag, char const *value, params_t *params);
bool check_client(char const *flag, char const *value, params_t *params);
bool check_freq(char const *flag, char const *value, params_t *params);

/* unified_poll.c */
unified_poll_t *init_unified_poll(void);
void free_unified_poll(unified_poll_t *poll_struct);
int add_fd_to_poll(unified_poll_t *poll_struct, int fd, short events);
int remove_fd_from_poll(unified_poll_t *poll_struct, int fd);
void rebuild_poll_fds(zappy_t *zappy);
void poll_all_clients(zappy_t *zappy);
void free_action_queue_pl(action_queue_t *queue);

/* build_fds.c */
void rebuild_poll_fds(zappy_t *zappy);


/* signal.c */
void setup_signal(void);
int *get_running_state(void);

/* params.c */
params_t *check_args(int argc, char **argv);
void *free_params(params_t *params);

/* params_cherckers.c */
bool validate_no_extra_args(int argc, char **argv);

/* server.c */
zappy_t *init_server(int argc, char **argv);
void *free_zappy(zappy_t *server);

/* protocol.c */
int start_protocol(zappy_t *server);

/* client.c */
bool process_new_client(const char *team_name, int fd, zappy_t *server);
team_t *add_client_to_team(const char *team_name, int fd, zappy_t *server);
void check_player_status(zappy_t *zappy);
void remove_player_by_fd(zappy_t *zappy, int fd);
void remove_player_from_team(team_t *team, player_t *player, int fd,
    zappy_t *zappy);

/* init_map.c */
int init_game(zappy_t *server);
int distribute_resources(zappy_t *z);
map_t *create_map(int width, int height);

/* init_team.c */
int init_teams(zappy_t *server);

/* accept.c */
int accept_client(zappy_t *server);

/* refill_food.c */
void count_current_resources(zappy_t *z, int current_count[7]);
void refill_food(zappy_t *zappy);

/* free server  */
void *free_zappy(zappy_t *server);
void *free_params(params_t *params);
void *free_player(player_t *player);
void free_map(map_t *map);

/* free_server.c */
void free_networkplayer(player_t *player);
void free_players(player_t *player);
void free_teams(team_t *teams);


/* Function to send info to the gui */
int send_map_size(zappy_t *server);
int send_entrie_map(zappy_t *server);
int send_map_tile(inventory_t **tiles, zappy_t *server,
    int posX, int posY);
int send_team_name(zappy_t *server);
int send_egg(zappy_t *zappy, egg_t *egg);
int send_entire_egg_list(zappy_t *zappy);
int send_time_message(zappy_t *zappy);
int send_egg_death(zappy_t *zappy, egg_t *egg);
int send_egg_connect(zappy_t *zappy, egg_t *currentEgg);
int send_player_connect(zappy_t *zappy, player_t *player);
int send_player_pos(zappy_t *zappy, player_t *player);
int send_player_level(zappy_t *zappy, player_t *player);
int send_player_connect_to_specific_gui(graph_net_t *fd, player_t *p);
int send_player_inventory(zappy_t *zappy, player_t *player);
int send_player_expelled(zappy_t *zappy, player_t *player);
int send_broadcast_to_all(zappy_t *zappy, const char *message);
int send_broadcast_to_player(zappy_t *zappy, player_t *player,
    const char *message);
int send_player_laying_egg(zappy_t *zappy, player_t *player);
int send_ressource_droped(zappy_t *zappy, player_t *player,
    int ressourceType);
int send_ressource_collected(zappy_t *zappy, player_t *player,
    int ressourceType);
int send_player_death(zappy_t *zappy, player_t *player);
int send_updated_time(zappy_t *zappy, int time);
int send_end_game(zappy_t *zappy, const char *teamName);
int send_str_message(zappy_t *zappy, const char *message);
int send_unknown_command(zappy_t *zappy);
int send_command_parameter(zappy_t *zappy);
int send_start_incantation(zappy_t *zappy, player_t *player, int *player_list,
    int nb_player);
int send_end_incantation(zappy_t *zappy, player_t *player, char *result);

/* init_egg.c */
int init_egg(zappy_t *zappy);
egg_t *add_egg_node(int id, int *pos, char *team_name, int id_layer);
egg_t *kil_egg_node(egg_t **head, int egg_id);

/* AI messages */
int forward_message(player_t *player, params_t *params);

/* Pollin handler */
void process_player_actions(player_t *player, zappy_t *zappy);
void process_player_actions_tick(zappy_t *zappy);
void execute_action(player_t *player, action_request_t *action,
    zappy_t *zappy);
void queue_action(player_t *player, char *command, zappy_t *zappy);
action_queue_t *init_action_queue(void);
void free_action_queue(action_queue_t *queue);
action_request_t *create_action_request(char *command, player_t *player,
    int frequency);
const command_info_t *find_command_info(char *command);
action_request_t *dequeue_highest_priority_action(action_queue_t *queue);
void free_action_request(action_request_t *action);
void insert_action_by_priority(action_queue_t *queue,
    action_request_t *action);

/* Unified polling functions */
unified_poll_t *init_unified_poll(void);
void free_unified_poll(unified_poll_t *poll_struct);
int add_fd_to_poll(unified_poll_t *poll_struct, int fd, short events);
int remove_fd_from_poll(unified_poll_t *poll_struct, int fd);
void poll_all_clients(zappy_t *zappy);
void rebuild_poll_fds(zappy_t *zappy);

/* This is the definition of the array function of the commands */
int handle_forward(player_t *player, char *command, zappy_t *zappy);

int handle_left(player_t *player, char *command, zappy_t *zappy);
int left_message(player_t *player);
int print_left_server(player_t *player);

int handle_right(player_t *player, char *command, zappy_t *zappy);
int print_right_server(player_t *player);
int right_message(player_t *player);

int handle_connect_nbr(player_t *player, char *command, zappy_t *zappy);
int handle_eject(player_t *player, char *command, zappy_t *zappy);

/* fork */
int handle_fork(player_t *player, char *command, zappy_t *zappy);
int handle_fork_end(player_t *player, zappy_t *zappy);
void push_back_egg(zappy_t *zappy, egg_t *new);
int print_look_server(player_t *player);

/* Incantation handler */
int handle_incantation(player_t *player, char *command, zappy_t *zappy);
int check_player_on_tile(player_t *player, zappy_t *zappy);
void increase_level_player(int *player_list, int nb_players, zappy_t *zappy);
int *get_player_on_tile_id(int posX, int posY, zappy_t *zappy, int nb_players);
int handle_end_incantation(player_t *player, zappy_t *zappy);
int get_nb_player_on_tile(int posX, int posY, zappy_t *zappy, int level);
void mark_players_incanting(int *player_list, int nb_players, zappy_t *zappy);
void remove_crystal_from_tiles(int posX, int posY, int level, zappy_t *zappy);
int validate_and_get_players(player_t *player, zappy_t *zappy,
    int **player_list);


int handle_inventory(player_t *player, char *command, zappy_t *zappy);
int inventory_message(player_t *player);
int print_inventory_server(player_t *player, int len);

int handle_broadcast(player_t *player, char *command, zappy_t *zappy);
int broadcast_text(player_t *source, player_t *dest, char *text,
    zappy_t *zappy);

int handle_look(player_t *player, char *command, zappy_t *zappy);
int handle_set(player_t *player, char *command, zappy_t *zappy);
int handle_take(player_t *player, char *command, zappy_t *zappy);

/* graphic_clinet.c */
graph_net_t *add_graph_node(graph_net_t **head, int fd);
graph_net_t *remove_graph_node(graph_net_t **head, int fd);
int poll_graphic_commands(zappy_t *zappy, graph_net_t *current,
    char *buffer);


/* Element hander.c */
void add_food(inventory_t *inventory);
void add_linemate(inventory_t *inventory);
void add_deraumere(inventory_t *inventory);
void add_sibur(inventory_t *inventory);
void add_mendiane(inventory_t *inventory);
void add_phiras(inventory_t *inventory);
void add_thystame(inventory_t *inventory);

void rm_food(inventory_t *inventory);
void rm_linemate(inventory_t *inventory);
void rm_deraumere(inventory_t *inventory);
void rm_sibur(inventory_t *inventory);
void rm_mendiane(inventory_t *inventory);
void rm_phiras(inventory_t *inventory);
void rm_thystame(inventory_t *inventory);

/* Element handler.c */
int msz(zappy_t *zappy, graph_net_t *graphic, char *message);
int bct(zappy_t *zappy, graph_net_t *graphic, char *message);
int mct(zappy_t *zappy, graph_net_t *graphic, char *message);
int tna(zappy_t *zappy, graph_net_t *graphic, char *message);
int ppo(zappy_t *zappy, graph_net_t *graphic, char *message);
int plv(zappy_t *zappy, graph_net_t *graphic, char *message);
int plu(zappy_t *zappy, graph_net_t *graphic, char *message);
int pld(zappy_t *zappy, graph_net_t *graphic, char *message);
int pin(zappy_t *zappy, graph_net_t *graphic, char *message);
int sgt(zappy_t *zappy, graph_net_t *graphic, char *message);
int sst(zappy_t *zappy, graph_net_t *graphic, char *message);
int kil(zappy_t *zappy, graph_net_t *graphic, char *message);
int tar(zappy_t *zappy, graph_net_t *graphic, char *message);
int tsr(zappy_t *zappy, graph_net_t *graphic, char *message);
int pia(zappy_t *zappy, graph_net_t *graphic, char *message);
int pis(zappy_t *zappy, graph_net_t *graphic, char *message);
int send_bct_message(graph_net_t *graphic, int x, int y,
    inventory_t *inventory);
int send_pin_message(graph_net_t *graphic, player_t *player);

/* player_id.c */
player_t *get_player_by_id(game_t *game, int player_id);
int get_next_free_id(zappy_t *server);
void verify_player_id(zappy_t *zappy, player_t *player);

/* pending_connections.c */
bool is_pending_connection(zappy_t *zappy, int fd);
int accept_client_team_name(zappy_t *zappy, int new_sockfd);
#endif /* !ZAPPY_H_ */
