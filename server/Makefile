##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Makefile
##

GREEN=\033[1;32m
BLUE=\033[1;34m
RED=\033[1;31m
YELLOW=\033[1;33m
NC=\033[0m


MAIN 		= 	src/main.c

MAIN_OBJ 	= 	$(MAIN:.c=.o)

SRC 		=	src/params.c										\
				src/messages.c										\
				src/checkers.c										\
				src/server.c										\
				src/protocol.c										\
				src/pending_connections.c							\
				src/game/client.c									\
				src/game/game.c										\
				src/game/init_egg.c									\
				src/game/player.c									\
				src/game/player_id.c 								\
				src/game/graphic_client.c							\
				src/game/handle_crystal.c 							\
				src/game/handle_food.c         						\
				src/game/handle_remove.c 							\
				src/game/food_refiil.c 								\
				src/game/get_ids_player_tile.c 						\
				src/game/init_team.c 								\
				src/game/init_map.c									\
				src/free.c											\
				src/free_player.c									\
				src/algo.c											\
				src/accept.c										\
				src/signal.c										\
				src/params_checkers.c 								\
				src/guiMessage/gui_send_map_data.c					\
				src/guiMessage/gui_send_team_name.c					\
				src/guiMessage/gui_send_eggs.c						\
				src/guiMessage/gui_send_time.c						\
				src/guiMessage/gui_send_egg_death.c 				\
				src/guiMessage/gui_send_egg_connect.c				\
				src/guiMessage/gui_send_player_info.c				\
				src/guiMessage/gui_send_rest_info_player.c			\
				src/guiMessage/gui_miscellaneous_send.c				\
				src/guiMessage/gui_send_boradcast.c					\
				src/guiMessage/gui_send_death_player.c				\
				src/guiMessage/gui_send_ressource.c					\
				src/guiMessage/gui_send_start_incantation.c 		\
				src/guiMessage/gui_send_end_incantation.c			\
				src/guiMessage/handlers/msz.c						\
				src/guiMessage/handlers/bct.c						\
				src/guiMessage/handlers/mct.c						\
				src/guiMessage/handlers/tna.c						\
				src/guiMessage/handlers/ppo.c						\
				src/guiMessage/handlers/plv.c						\
				src/guiMessage/handlers/pin.c						\
				src/guiMessage/handlers/sgt.c						\
				src/guiMessage/handlers/sst.c						\
				src/guiMessage/handlers/kil.c						\
				src/guiMessage/handlers/pld.c						\
				src/guiMessage/handlers/plu.c						\
				src/guiMessage/handlers/tar.c						\
				src/guiMessage/handlers/tsr.c						\
				src/guiMessage/handlers/pia.c						\
				src/guiMessage/handlers/pis.c						\
				src/receiveMessage/action_handler.c					\
				src/receiveMessage/pollin_handler.c					\
				src/receiveMessage/loop_thru_command.c				\
				src/receiveMessage/queu_hanlder.c					\
				src/receiveMessage/unified_polling.c				\
				src/receiveMessage/build_fds.c      				\
				src/receiveMessage/init_poll_fds.c 					\
				src/aiMessage/end_incantation.c 					\
				src/aiMessage/forward.c								\
				src/aiMessage/right.c								\
				src/aiMessage/left.c								\
				src/aiMessage/broadcast.c							\
				src/aiMessage/connect.c								\
				src/aiMessage/eject.c								\
				src/aiMessage/incantation.c							\
				src/aiMessage/inventory.c							\
				src/aiMessage/set_object.c  						\
				src/aiMessage/take_object.c							\
				src/aiMessage/fork.c								\
				src/aiMessage/look.c

OBJ 		= 	$(SRC:.c=.o)

INCLUDES 	= 	-I./include

CFLAGS 		=	-Wall -Wextra -Werror -Wshadow

LIBS 		= -L./ -lmy -lnetwork -lm

NAME 		= 	zappy_server

CC 			= 	gcc

CRIT 		= 	-lcriterion --coverage

all: libs $(NAME)


libs:
	$(MAKE) -C lib/my/
	$(MAKE) -C src/network/

$(NAME): $(OBJ) $(MAIN_OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Compiling $(NAME)...$(NC)"
	@$(CC) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(CFLAGS) $(INCLUDES) $(LIBS)
	@echo -e "$(GREEN)ᒥ✅ᒧ Compilation completed successfully!$(NC)"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@rm -rf $(MAIN_OBJ)
	@rm -rf *.gc*
	-rm libmy.a
	-rm libnetwork.a

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT)
	@rm -f tests/*.c
	-rm libmy.a
	-rm libnetwork.a

re: fclean all

.PHONY: all clean fclean re
