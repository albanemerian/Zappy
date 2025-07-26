##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Makefile
##

NAME = libSFMLAudio.so

SRC =	../AAudio.cpp	\
		SFMLAudio.cpp	\
		entryPoint.cpp

CXX = g++

CFLAGS = -std=c++20 -Wall -Wextra -Wshadow -Werror -O2 -fPIC -lsfml-audio

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Creating shared library $(NAME)...$(NC)"
	$(CXX) -shared -o $(NAME) $(OBJ) $(ENCAPPS) $(CFLAGS)
	@echo -e "$(GREEN)ᒥ✅ᒧ Library $(NAME) created successfully!$(NC)"

clean:
	@rm -rf $(OBJ)
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.gcov
	@echo -e "$(YELLOW)ᒥ🧹ᒧ Cleaned object files and coverage data!$(NC)"

fclean: clean
	@rm -rf $(NAME)
	@echo -e "$(RED)ᒥ🗑️ᒧ Library $(NAME) removed!$(NC)"

re: fclean all

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@
