#include "brick_game/tetris/game_controller/game_controller.h"

int main(void) {
  GameState *game_state = NULL;
  init_game(&game_state);
  run_game(game_state);
  cleanup_game(&game_state);
  return 0;
}
