##
## EPITECH PROJECT, 2025
## B-YEP-400-NAN-4-1-zappy-albane.merian
## File description:
## Makefile
##



SRC		=	my_itoa.c					\
			my_int_str_length.c			\
			my_unsignedlen.c			\
			is_only_digit.c				\

OBJ 	= 	$(SRC:.c=.o)

NAME	= 	libmy.a

CC 		= 	gcc

all:	$(NAME)

$(NAME):	$(OBJ)
		cp my.h ../../include
		@$(AR) rc $(NAME) $(OBJ)
		cp $(NAME) ../../
		cp $(NAME) ../../../tests/unit/server/
		@echo -e "\e[1;36m{ libmy.a ready and my.h duplicated !! }\e[00;37m"

clean:
	@rm -f $(OBJ)
	@rm -f *~
	@echo -e "\e[1;33m[ Useless files have been deleted ]\e[00;37m"

fclean: clean
	$(RM) $(NAME)
	-rm ../../../../tests/unit/server/$(NAME)
	@echo -e "\e[1;33m[ libmy.a has been deleted ]\e[00;37m"

re: fclean all
