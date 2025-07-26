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

UNIT 	    = 	unit_tests

TESTS 	    =   tests_accept.c						\
				tests_aiMessage.c					\
				tests_bct.c							\
				tests_buffer.c 						\
				tests_checkers.c					\
				tests_client.c						\
				tests_connect.c						\
				tests_errors.c						\
				tests_fork.c						\
				tests_free.c						\
				tests_get_objects.c					\
				tests_grapClients.c					\
				tests_gui_incantation.c				\
				tests_guiMessage.c					\
				tests_handler_elem.c				\
				tests_incantation.c 				\
				tests_inventory.c					\
				fake_malloc.c						\
				tests_left.c						\
				tests_look.c						\
				tests_malloc_fails.c				\
				tests_mct.c							\
				tests_messaggeReception.c			\
				tests_msz.c							\
				tests_params.c						\
				tests_pin.c							\
				tests_plv.c							\
				tests_ppo.c							\
				tests_right.c						\
				tests_server.c						\
				tests_setObj.c 						\
				tests_sgt.c							\
				tests_sst.c							\
				tests_tna.c							\
				tests_team.c						\
				tests_eject.c						\
				tests_map_init.c					\
				tests_kil.c							\
				tests_network.c						\
				tests_broadcast.c					\
				tests_message_handlers.c			\
				tests_pollfds.c						\
				tests_freePlayer.c					\
				tests_player.c


SRC 		=	../../../server/src/accept.c								\
				../../../server/src/algo.c									\
				../../../server/src/checkers.c								\
				../../../server/src/free.c									\
				../../../server/src/free_player.c							\
				../../../server/src/messages.c								\
				../../../server/src/params_checkers.c 						\
				../../../server/src/params.c								\
				../../../server/src/protocol.c								\
				../../../server/src/pending_connections.c					\
				../../../server/src/receiveMessage/unified_polling.c		\
				../../../server/src/receiveMessage/build_fds.c      		\
				../../../server/src/server.c								\
				../../../server/src/signal.c								\
				../../../server/src/game/client.c							\
				../../../server/src/game/game.c								\
				../../../server/src/game/init_map.c							\
				../../../server/src/game/graphic_client.c					\
				../../../server/src/game/handle_crystal.c 					\
				../../../server/src/game/handle_food.c						\
				../../../server/src/game/handle_remove.c 					\
				../../../server/src/game/init_egg.c							\
				../../../server/src/game/player.c							\
				../../../server/src/game/player_id.c 						\
				../../../server/src/game/get_ids_player_tile.c 				\
				../../../server/src/game/init_team.c 						\
				../../../server/src/game/food_refiil.c 						\
				../../../server/src/network/accept_connection.c				\
				../../../server/src/network/buffer.c						\
				../../../server/src/network/close.c							\
				../../../server/src/network/create_server.c					\
				../../../server/src/network/hanlde_input.c					\
				../../../server/src/network/print_error.c					\
				../../../server/src/network/send_message.c					\
				../../../server/src/guiMessage/gui_miscellaneous_send.c		\
				../../../server/src/guiMessage/gui_send_boradcast.c			\
				../../../server/src/guiMessage/gui_send_death_player.c		\
				../../../server/src/guiMessage/gui_send_egg_connect.c		\
				../../../server/src/guiMessage/gui_send_egg_death.c 		\
				../../../server/src/guiMessage/gui_send_eggs.c				\
				../../../server/src/guiMessage/gui_send_end_incantation.c	\
				../../../server/src/guiMessage/gui_send_map_data.c			\
				../../../server/src/guiMessage/gui_send_player_info.c		\
				../../../server/src/guiMessage/gui_send_ressource.c			\
				../../../server/src/guiMessage/gui_send_rest_info_player.c	\
				../../../server/src/guiMessage/gui_send_start_incantation.c \
				../../../server/src/guiMessage/gui_send_team_name.c			\
				../../../server/src/guiMessage/gui_send_time.c				\
				../../../server/src/guiMessage/handlers/bct.c				\
				../../../server/src/guiMessage/handlers/mct.c				\
				../../../server/src/guiMessage/handlers/msz.c				\
				../../../server/src/guiMessage/handlers/pin.c				\
				../../../server/src/guiMessage/handlers/plv.c				\
				../../../server/src/guiMessage/handlers/ppo.c				\
				../../../server/src/guiMessage/handlers/sgt.c				\
				../../../server/src/guiMessage/handlers/sst.c				\
				../../../server/src/guiMessage/handlers/tna.c				\
				../../../server/src/guiMessage/handlers/kil.c				\
				../../../server/src/guiMessage/handlers/pld.c				\
				../../../server/src/guiMessage/handlers/plu.c				\
				../../../server/src/guiMessage/handlers/tar.c				\
				../../../server/src/guiMessage/handlers/tsr.c				\
				../../../server/src/guiMessage/handlers/pia.c				\
				../../../server/src/guiMessage/handlers/pis.c				\
				../../../server/src/receiveMessage/action_handler.c			\
				../../../server/src/receiveMessage/loop_thru_command.c		\
				../../../server/src/receiveMessage/pollin_handler.c			\
				../../../server/src/receiveMessage/queu_hanlder.c			\
				../../../server/src/receiveMessage/init_poll_fds.c      	\
				../../../server/src/aiMessage/broadcast.c					\
				../../../server/src/aiMessage/connect.c						\
				../../../server/src/aiMessage/eject.c						\
				../../../server/src/aiMessage/end_incantation.c				\
				../../../server/src/aiMessage/fork.c						\
				../../../server/src/aiMessage/forward.c						\
				../../../server/src/aiMessage/incantation.c					\
				../../../server/src/aiMessage/inventory.c					\
				../../../server/src/aiMessage/left.c						\
				../../../server/src/aiMessage/look.c						\
				../../../server/src/aiMessage/right.c						\
				../../../server/src/aiMessage/set_object.c  				\
				../../../server/src/aiMessage/take_object.c

INCLUDES 	= 	-I./../../../server/include -I./

CFLAGS 		=	-Wall -Wextra

LIBS 		=	-L. -lmy -lnetwork -lm

CC 			= 	gcc

CRIT 		= 	-lcriterion --coverage

# Build fake malloc with control functions
fake_malloc:
	@echo -e "$(YELLOW)ᒥ🧪ᒧ Generating fake malloc with control functions...$(NC)"
	@gcc -fPIC -shared -o libfailalloc.so fake_malloc.c -ldl

clean:
	@rm -rf $(OBJ)
	@rm -rf $(MAIN_OBJ)
	@rm -rf *.gc*
	@rm -rf libfailalloc.so

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT)

unit_test: fclean fake_malloc
	@echo -e "$(YELLOW)ᒥ🧪ᒧ Compiling unit tests with fake malloc support...$(NC)"
	@$(CC) -o $(UNIT) $(SRC) $(TESTS) $(CFLAGS) $(INCLUDES) $(CRIT) $(LIBS)

tests_run: unit_test
	@echo -e "$(YELLOW)ᒥ🧪ᒧ Running unit tests with malloc control...$(NC)"
	LD_PRELOAD=./libfailalloc.so ./$(UNIT)

coverage: tests_run
	@echo -e "$(BLUE)ᒥ📊ᒧ Generating coverage report...$(NC)"
	@gcovr --root ../../../ --exclude tests/* --exclude-unreachable-branches

re: fclean all

.PHONY: all clean fclean re
