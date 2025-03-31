#include "game_controller.h"

UserAction_t process_user_input(int key) {
  switch (key) {
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_DOWN:
      return Down;
    case 10:  // Enter
      return Action;
    case 'p':
    case 'P':
      return Pause;
    case 'q':
    case 'Q':
      return Terminate;
    case ' ':
      return Start;
    default:
      return 0;
  }
}

int init_game(GameState **state) {
  // Инициализация рандома
  init_random();

  // Инициализация ncurses
  init_ncurses();

  // Создание состояния игры
  *state = init_game_state();
  if (*state == NULL) {
    cleanup_ncurses();
    return 1;
  }

  return 0;
}

int run_game(GameState *state) {
  int running = 1;

  while (running) {
    int key = getch();
    if (key != ERR) {
      UserAction_t action = process_user_input(key);
      if (action == Terminate) {
        running = 0;
      } else {
        userInput(state, action, 0);
      }
    }

    updateCurrentState(state);
    print_game_info(state);
    napms(REFRESH_DELAY);
  }

  return 0;
}

void cleanup_game(GameState **state) {
  if (*state != NULL) {
    destroy_game_state(*state);
    cleanup_ncurses();
    *state = NULL;
  }
}