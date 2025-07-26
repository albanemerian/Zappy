##
## EPITECH PROJECT, 2025
## B-OOP-400-NAN-4-1-raytracer-albane.merian
## File description:
## Makefile
##

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -O2 -fPIC

# Directories
SRC_DIR = .
OBJ_DIR = obj
INC_DIR = .

# Files
SRC = $(wildcard $(SRC_DIR)/*.cpp)

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

LIB_STATIC = libDLLoader.a

all: $(LIB_STATIC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(LIB_STATIC): $(OBJ)
	ar rcs $@ $^

$(LIB_SHARED): $(OBJ)
	$(CXX) -shared -o $@ $^

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(LIB_STATIC)

re: fclean all
