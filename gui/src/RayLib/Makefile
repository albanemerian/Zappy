##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Makefile
##

NAME = libZappyRayLib.so

SRC =	Raylib.cpp	\
		entryPoint.cpp

CXX = g++

ENCAPPS = RaylibEnc/libZappyRayLibCpp.a

CFLAGS = -std=c++20 -Wall -Wextra -Wshadow -Werror -O2 -fPIC -lraylib

OBJ = $(SRC:.cpp=.o)


all: $(NAME)

$(NAME): $(ENCAPPS) $(OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Creating shared library $(NAME)...$(NC)"
	$(CXX) -shared -o $(NAME) $(OBJ) $(ENCAPPS) $(CFLAGS)
	@echo -e "$(GREEN)ᒥ✅ᒧ Library $(NAME) created successfully!$(NC)"

$(ENCAPPS):
	$(MAKE) -C RaylibEnc
	@echo -e "$(GREEN)ᒥ✅ᒧ Library $(ENCAPPS) created successfully!$(NC)"*

clean:
	@rm -rf $(OBJ)
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.gcov
	$(MAKE) -C RaylibEnc clean
	@echo -e "$(YELLOW)ᒥ🧹ᒧ Cleaned object files and coverage data!$(NC)"

fclean: clean
	@rm -rf $(NAME)
	$(MAKE) -C RaylibEnc fclean
	@echo -e "$(RED)ᒥ🗑️ᒧ Library $(NAME) removed!$(NC)"

re: fclean all

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@
