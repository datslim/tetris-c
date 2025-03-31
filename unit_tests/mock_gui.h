#ifndef MOCK_GUI_H
#define MOCK_GUI_H

#include "../brick_game/tetris/game_state/game_state.h"
#include "../brick_game/tetris/game_tetromino/game_tetromino.h"

void init_ncurses(void);
void cleanup_ncurses(void);
void print_game_info(GameState *state);
void print_game_field(const GameInfo_t *game_info);
void print_next_tetromino(Tetromino_t *next_tetromino);
void print_game_over(void);
void print_pause(void);
void print_unpause(void);
void handle_input(GameState *state, int key);

#endif  // MOCK_GUI_H