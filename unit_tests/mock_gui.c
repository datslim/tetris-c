#include "mock_gui.h"

#include "../brick_game/tetris/game_controller/game_controller.h"
#include "../brick_game/tetris/game_state/game_state.h"

void init_ncurses(void) {
  // Mock implementation
}

void cleanup_ncurses(void) {
  // Mock implementation
}

void print_game_info(GameState *state __attribute__((unused))) {
  // Mock implementation
}

void print_game_field(const GameInfo_t *game_info __attribute__((unused))) {}

void print_next_tetromino(Tetromino_t *next_tetromino __attribute__((unused))) {
  // Mock implementation
}

void print_game_over(void) {
  // Mock implementation
}

void print_pause(void) {}
void print_unpause(void) {}

void handle_input(GameState *state, int key) {
  if (!state) return;

  switch (key) {
    case 'p':
    case 'P':
      state->game_info.pause = !state->game_info.pause;
      break;
    case KEY_LEFT:
      if (can_move_left(state)) {
        move_left(state);
      }
      break;
    case KEY_RIGHT:
      if (can_move_right(state)) {
        move_right(state);
      }
      break;
    case KEY_DOWN:
      if (can_move_down(state)) {
        move_down(state);
      }
      break;
    case 10:  // Enter
      if (can_rotate(state)) {
        rotate_tetromino(state);
      }
      break;
  }
}