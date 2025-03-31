CC=gcc
CFLAGS=-Wall -Wextra -Werror -I./brick_game/common
LDFLAGS=-lncurses 
CHECKFLAGS=-lcheck -lpthread -lrt -lm -lsubunit
GCOVFLAGS=-fprofile-arcs -ftest-coverage -lgcov
SRC_DIR=.
TETRIS_DIR=$(SRC_DIR)/brick_game/tetris
GUI_DIR=$(SRC_DIR)/gui/cli
BUILD_DIR=./build
OBJ_DIR=$(BUILD_DIR)/obj
DOXYGEN_DIR=doxygen_files
TEST_DIR=$(SRC_DIR)/unit_tests

TETRIS_SRCS=$(filter-out $(TETRIS_DIR)/game_controller/game_controller.c, $(wildcard $(TETRIS_DIR)/*/*.c $(TETRIS_DIR)/*.c))
GAME_SRCS=$(TETRIS_SRCS) $(TETRIS_DIR)/game_controller/game_controller.c
GUI_SRCS=$(wildcard $(GUI_DIR)/*.c)
MAIN_SRC=$(SRC_DIR)/main.c
TEST_SRCS = unit_tests/test_main.c \
            unit_tests/test_game_state.c \
            unit_tests/test_game_field.c \
            unit_tests/test_game_scoring.c \
            unit_tests/test_game_tetromino.c

TEST_TETRIS_SRCS = $(TETRIS_DIR)/game.c \
                   $(TETRIS_DIR)/game_field/game_field.c \
                   $(TETRIS_DIR)/game_scoring/game_scoring.c \
                   $(TETRIS_DIR)/game_state/game_state.c \
                   $(TETRIS_DIR)/game_tetromino/game_tetromino.c

TETRIS_OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(TETRIS_SRCS))
GUI_OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(GUI_SRCS))
MAIN_OBJ=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(MAIN_SRC))
TEST_OBJS = $(TEST_SRCS:%.c=build/obj/%.o)
GAME_OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(GAME_SRCS))
TEST_TETRIS_OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_TETRIS_SRCS))

all: clean run_game

tetris.a: $(GAME_OBJS) $(GUI_OBJS) $(MAIN_OBJ)
	ar rcs tetris.a $(OBJ_DIR)/*.o 
	ranlib tetris.a

run_game: install
	./$(BUILD_DIR)/tetris

install: $(GAME_OBJS) $(GUI_OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $(BUILD_DIR)/tetris

uninstall:
	rm -rf $(BUILD_DIR)

test: $(TEST_TETRIS_OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_TETRIS_OBJS) $(TEST_OBJS) $(LDFLAGS) $(CHECKFLAGS) -o $(BUILD_DIR)/test
	./$(BUILD_DIR)/test

gcov_report: clean
	rm -rf gcov
	mkdir gcov
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $(TEST_TETRIS_SRCS) $(TEST_SRCS)
	$(CC) $(CFLAGS) $(GCOVFLAGS) *.o -o $(BUILD_DIR)/testgcov $(LDFLAGS) $(CHECKFLAGS)
	./$(BUILD_DIR)/testgcov
	lcov --ignore-errors empty --no-external -t "gcov_report" -o gcov/brick_game_test.info -c -d .
	genhtml -o report gcov/brick_game_test.info
	rm -rf testgcov *.o *.gcno *.gcda
	xdg-open report/index.html

dvi:
	doxygen doxygen/Doxyfile
	cp ./doxygen/images/game_screenshot.png ./doxygen_files/html/
	cp ./tetris-FSM.png ./doxygen_files/html/
	xdg-open $(DOXYGEN_DIR)/html/index.html

dist: clean format
	@mkdir -p dist
	@cp -r \
		Makefile \
		doxygen/ \
		tetris-FSM.png \
		$(SRC_DIR)/brick_game/ \
		$(SRC_DIR)/gui/ \
		main.c \
		unit-tests/ \
		dist
	@tar -cf tetris-dist.tar dist
	@gzip tetris-dist.tar
	@rm -rf dist
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/unit-tests/%.o: $(SRC_DIR)/unit-tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

format-check:
	cp ../materials/linters/.clang-format .
	clang-format -n */*.c */*.h *.c
	clang-format -n brick_game/tetris/*/*.c brick_game/tetris/*/*.h *.c
	rm -f .clang-format

format:
	cp ../materials/linters/.clang-format .
	clang-format -i */*.c */*.h *.c
	clang-format -i brick_game/tetris/*/*.c brick_game/tetris/*/*.h *.c
	rm -f .clang-format

clean: uninstall
	rm -rf $(DOXYGEN_DIR)
	rm -rf gcov
	rm -rf report
	rm -f tetris-dist.tar.gz
	rm -f *.txt
	rm -f *.gcno
	rm -f *.o
	rm -rf *.gcda
	rm -rf *.a
	
.PHONY: all clean install uninstall dvi dist test format format-check