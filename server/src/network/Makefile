##
## EPITECH PROJECT, 2025
## B-YEP-400-NAN-4-1-zappy-albane.merian
## File description:
## Makefile
##

SRC		=	accept_connection.c	\
			close.c				\
			create_server.c		\
			hanlde_input.c		\
			send_message.c		\
			buffer.c			\
			print_error.c		\
			print_buffer.c		\
			empty_buffer.c

OBJ 	= 	$(SRC:.c=.o)

NAME 	= 	libnetwork.a

CC 		= 	gcc

all:	$(NAME)

$(NAME):	$(OBJ)
	cp *.h ../../include
	@$(AR) rc $(NAME) $(OBJ)
	cp $(NAME) ../../
	cp $(NAME) ../../../tests/unit/server/
	@echo -e "\e[1;36m{ libs duplicated !! }\e[00;37m"

clean:
	@rm -f $(OBJ)
	@rm -f *~
	@echo -e "\e[1;33m[ Useless files have been deleted ]\e[00;37m"

fclean: clean
	$(RM) $(NAME)
	-rm ../../../tests/unit/server/$(NAME)
	@echo -e "\e[1;33m[ libnetwork.a has been deleted ]\e[00;37m"

re: fclean all
