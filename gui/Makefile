##
## EPITECH PROJECT, 2025
## zappy
## File description:
## GUI Makefile
##

# ANSI Colors
GREEN=\033[1;32m
BLUE=\033[1;34m
RED=\033[1;31m
YELLOW=\033[1;33m
NC=\033[0m

MAIN = src/main.cpp

MAIN_OBJ = $(MAIN:.cpp=.o)

SRC =	src/Client/Client.cpp \
		src/Client/MsgHandler.cpp \
		src/CLI/CLI.cpp \
		src/Game/GameInfos.cpp \
		src/Graphic/GUI.cpp \
		src/Graphic/Map.cpp \
		src/Graphic/SplashScreen.cpp \
		src/Graphic/Camera/CameraManager.cpp \
		src/Graphic/HUD/Button/Button.cpp \
		src/Graphic/HUD/Containers/Containers.cpp \
		src/Graphic/HUD/Containers/AContainers.cpp \
		src/Graphic/HUD/Text/Text.cpp \
		src/Graphic/HUD/Slider/Slider.cpp \
		src/Graphic/HUD/Checkbox/Checkbox.cpp \
		src/Graphic/HUD/Image/Image.cpp \
		src/Graphic/HUD/ImageButton/ImageButton.cpp \
		src/Graphic/HUD/UIElement/AUIElement.cpp \
		src/Graphic/HUD/HUD.cpp	\
		src/Observer/GuiObserver.cpp \
		src/Observer/Subject.cpp \
		src/Graphic/HUD/Help/Help.cpp \
		src/Graphic/HUD/Settings/Settings.cpp

OBJ = $(SRC:.cpp=.o)

TEST_SRC = ../tests/unit/gui/main_test.cpp \
		   ../tests/unit/gui/CLI/CLI_test.cpp \
		   ../tests/unit/gui/Exceptions/Exceptions_test.cpp \
		   ../tests/unit/gui/Client/Client_test.cpp \
		   ../tests/unit/gui/Communication/Communication_test.cpp \
		   ../tests/unit/gui/Game/GameInfos_test.cpp \

TEST_OBJ = $(TEST_SRC:.cpp=.o)

INCLUDES = -I./include -I./src

CFLAGS =	-std=c++20 -Wall -Wextra -Wshadow -Werror -O2  -L./src/DLLoader/ \
	-lDLLoader
CFLAGS_COVERAGE = $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage

LIBS = -L./src/Communication -lcommunication \
		-L./src/DLLoader/ -lDLLoader -lsfml-audio \
		-lGL -lm -lpthread -ldl -lrt -lX11

COMM_LIB = src/Communication/libcommunication.a
RAYLIB_LIB = src/RayLib/libZappyRayLib.so
AUDIO_LIB = src/Audio/SFML/libSFMLAudio.so
DLLoader_LIB = src/DLLoader/libDLLoader.a

NAME = zappy_gui

UNIT_TESTS = unit_tests

CC = g++

all: $(NAME)

$(NAME): $(COMM_LIB) $(RAYLIB_LIB) $(AUDIO_LIB) $(DLLoader_LIB) \
 	$(OBJ) $(MAIN_OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Compiling $(NAME)...$(NC)"
	$(CC) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(CFLAGS) $(INCLUDES) $(LIBS)
	@echo -e "$(GREEN)ᒥ✅ᒧ Compilation completed successfully!$(NC)"

$(COMM_LIB):
	@echo -e "$(BLUE)ᒥ🔧ᒧ Building Communication library...$(NC)"
	@make -C src/Communication

$(RAYLIB_LIB):
	@echo -e "$(BLUE)ᒥ🔧ᒧ Building RayLib wrapper library...$(NC)"
	@make -C src/RayLib
	@echo -e "$(GREEN)ᒥ✅ᒧ Library $(RAYLIB_LIB) created successfully!$(NC)"
	@mkdir -p ./lib
	@cp $(RAYLIB_LIB) ./lib/

$(DLLoader_LIB):
	@echo -e "$(BLUE)ᒥ🔧ᒧ Building DLLoader library...$(NC)"
	@make -C src/DLLoader

$(AUDIO_LIB):
	@echo -e "$(BLUE)ᒥ🔧ᒧ Building Audio library...$(NC)"
	@make -C src/Audio
	@cp $(AUDIO_LIB) ./lib/

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@rm -rf $(MAIN_OBJ)
	@make -C src/Communication clean
	@make -C src/RayLib clean
	@make -C src/Audio clean
	@make -C src/DLLoader clean

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT_TESTS)
	@rm -rf ./lib
	@rm -rf coverage_report/
	@find . -name "*.gcda" -type f -delete
	@find . -name "*.gcno" -type f -delete
	@find . -name "*.gcov" -type f -delete
	@make -C src/Communication fclean
	@make -C src/RayLib fclean
	@make -C src/Audio fclean
	@make -C src/DLLoader fclean

re: fclean all

coverage: tests_run
	@echo -e "$(BLUE)ᒥ📊ᒧ Generating coverage report...$(NC)"
	@mkdir -p coverage_report
	@gcovr --root . --exclude tests/ --exclude-unreachable-branches \
	--merge-mode-functions=merge-use-line-min \
	--html --html-details -o coverage_report/index.html
	@echo -e "$(GREEN)ᒥ✅ᒧ Report generated in coverage_report/index.html$(NC)"
	@firefox coverage_report/index.html &> /dev/null || xdg-open \
		coverage_report/index.html &> /dev/null || echo "Please open \
		coverage_report/index.html in your browser"

tests_run: communication_lib audio_lib $(SRC) $(TEST_SRC)
	@echo -e "$(BLUE)ᒥ🧪ᒧ Compiling Google Test unit tests...$(NC)"
	$(CC) -o $(UNIT_TESTS) $(SRC) $(TEST_SRC) $(CFLAGS_COVERAGE) $(INCLUDES) \
		$(LIBS) -lgtest -lgtest_main
	@echo -e "$(GREEN)ᒥ✅ᒧ Unit tests compiled! Running...$(NC)"
	./$(UNIT_TESTS)
	@echo -e "$(GREEN)ᒥ✅ᒧ All tests completed!$(NC)"

.PHONY: all clean fclean re communication_lib raylib_lib audio_lib coverage \
	tests_run
