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

SRC =	../../../gui/src/Client/Client.cpp \
		../../../gui/src/Client/MsgHandler.cpp \
		../../../gui/src/CLI/CLI.cpp \
		../../../gui/src/Communication/Communication.cpp \
		../../../gui/src/Game/GameInfos.cpp \
		../../../gui/src/Graphic/GUI.cpp \
		../../../gui/src/Graphic/Map.cpp \
		../../../gui/src/Graphic/SplashScreen.cpp \
		../../../gui/src/Graphic/HUD/Button/Button.cpp \
		../../../gui/src/Graphic/HUD/Containers/Containers.cpp \
		../../../gui/src/Graphic/HUD/Containers/AContainers.cpp \
		../../../gui/src/Graphic/HUD/Text/Text.cpp \
		../../../gui/src/Graphic/HUD/Slider/Slider.cpp \
		../../../gui/src/Graphic/HUD/Checkbox/Checkbox.cpp \
		../../../gui/src/Graphic/HUD/UIElement/AUIElement.cpp \
		../../../gui/src/Graphic/HUD/HUD.cpp \
		../../../gui/src/Graphic/Camera/CameraManager.cpp \
		../../../gui/src/Graphic/HUD/Settings/Settings.cpp \
		../../../gui/src/Graphic/HUD/Image/Image.cpp \
		../../../gui/src/Graphic/HUD/ImageButton/ImageButton.cpp \
		../../../gui/src/Graphic/HUD/Help/Help.cpp	 \
		../../../gui/src/Observer/GuiObserver.cpp	\
		../../../gui/src/Observer/Subject.cpp \

OBJ = $(SRC:.cpp=.o)

COV_OBJ = $(SRC:.cpp=_cov.o)

TEST_SRC = main_test.cpp \
		   CLI/CLI_test.cpp \
		   Exceptions/Exceptions_test.cpp \
		   Client/Client_test.cpp \
		   Client/MsgHandler_test.cpp \
		   Communication/Communication_test.cpp \
		   Game/GameInfos_test.cpp \
		   Camera_manager/Camera_manager_test.cpp \
		   Observer/Subject_test.cpp \
		   Observer/IObserver_test.cpp \
		   Observer/GuiObserver_test.cpp \

TEST_OBJ = $(TEST_SRC:.cpp=.o)

INCLUDES = -I./include -I../../../gui/include -I../../../gui/src

CFLAGS =	-std=c++20 -Wall -Wextra -Wshadow -Werror
CFLAGS_COVERAGE = $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage

LIBS = -L../../../gui/src/Communication -lcommunication \
		-lGL -lm -lpthread -ldl -lrt -lX11

NAME = zappy_gui

UNIT_TESTS = unit_tests

CC = g++

all: $(NAME)

$(NAME): libs $(OBJ) $(MAIN_OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Compiling $(NAME)...$(NC)"
	$(CC) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(CFLAGS) $(INCLUDES) $(LIBS)
	@echo -e "$(GREEN)ᒥ✅ᒧ Compilation completed successfully!$(NC)"

libs:
	@echo -e "$(BLUE)ᒥ🔧ᒧ Building libraries...$(NC)"
	@make -C ../../../gui/src/Communication
	@make -C ../../../gui/src/RayLib
	@make -C ../../../gui/src/Audio

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%_cov.o: %.cpp
	$(CC) $(CFLAGS_COVERAGE) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@rm -rf $(COV_OBJ)
	@rm -rf $(TEST_OBJ)
	@rm -rf $(MAIN_OBJ)
	@make -C ../../../gui/src/Communication clean
	@make -C ../../../gui/src/RayLib clean
	@make -C ../../../gui/src/Audio clean

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT_TESTS)
	@rm -rf coverage_report/
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.gcov
	@make -C ../../../gui/src/Communication fclean
	@make -C ../../../gui/src/RayLib fclean
	@make -C ../../../gui/src/Audio fclean

re: fclean all

coverage: tests_run
	@echo -e "$(BLUE)ᒥ📊ᒧ Generating coverage report...$(NC)"
	@mkdir -p coverage_report
	@gcovr --root ../../../gui/src --exclude=".*test.*" --exclude-unreachable-branches \
	--merge-mode-functions=merge-use-line-min \
	--html --html-details -o coverage_report/index.html
	@echo -e "$(GREEN)ᒥ✅ᒧ Report generated in coverage_report/index.html$(NC)"
	@firefox coverage_report/index.html &> /dev/null || xdg-open \
		coverage_report/index.html &> /dev/null || echo "Please open \
		coverage_report/index.html in your browser"

tests_run: libs $(COV_OBJ) $(TEST_OBJ)
	@echo -e "$(BLUE)ᒥ🧪ᒧ Compiling Google Test unit tests...$(NC)"
	$(CC) -o $(UNIT_TESTS) $(COV_OBJ) $(TEST_OBJ) $(CFLAGS_COVERAGE) $(INCLUDES) \
		$(LIBS) -lgtest -lgtest_main -lgmock -lgmock_main
	@echo -e "$(GREEN)ᒥ✅ᒧ Unit tests compiled! Running...$(NC)"
	./$(UNIT_TESTS)
	@echo -e "$(GREEN)ᒥ✅ᒧ All tests completed!$(NC)"

.PHONY: all clean fclean re libs
