##
## EPITECH PROJECT, 2025
## B-YEP-400-NAN-4-1-zappy-albane.merian
## File description:
## Makefile
##

# ANSI Colors
GREEN=\033[1;32m
BLUE=\033[1;34m
RED=\033[1;31m
YELLOW=\033[1;33m
NC=\033[0m

SRC =	RayLibEnc.cpp	\
		RaylibWindow.cpp	\
		RaylibInput.cpp	\
		Raylib3dEnv.cpp	\
		RaylibCamera.cpp	\
		RaylibGamepad.cpp \
		Raylib3dDrawing.cpp	\
		Raylib3dModel.cpp	\
		RaylibTextures.cpp	\
		RaylibSkybox.cpp	\
		RaylibCollision3D.cpp	\

OBJ = $(SRC:.cpp=.o)

INCLUDES = -I../../include -I../..

CFLAGS =	-std=c++20 -Wall -Wextra -Wshadow -Werror -O2 -fPIC
CFLAGS_COVERAGE = $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage

NAME = libZappyRayLibCpp.a

CC = g++

all: $(NAME)

$(NAME): $(OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Creating static library $(NAME)...$(NC)"
	ar rcs $(NAME) $(OBJ)
	@echo -e "$(GREEN)ᒥ✅ᒧ Library $(NAME) created successfully!$(NC)"

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.gcov

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
