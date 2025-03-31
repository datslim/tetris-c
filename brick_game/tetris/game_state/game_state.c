#include "game_state.h"

// Получить текущее время в миллисекундах
long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL);
  return te.tv_sec * 1000LL + te.tv_usec / 1000;
}

void userInput(GameState *state, UserAction_t action, int hold) {
  refresh();

  switch (state->state) {
    case STATE_START && !hold:
      if (action == Start) {
        state->state = STATE_SPAWN;
      }
      break;

    case STATE_GAME_OVER:
      if (action == Start) {
        restart_game(&state->game_info);  // инициализация в начальное состояние
        state->state = STATE_SPAWN;
      }
      break;

    case STATE_MOVE:
      if (action == Pause) {
        state->game_info.pause = !state->game_info.pause;
      } else if (!state->game_info.pause) {
        switch (action) {
          case Left:
            if (can_move_left(state)) {
              move_left(state);
            }
            break;

          case Right:
            if (can_move_right(state)) {
              move_right(state);
            }
            break;

          case Down:
            while (can_move_down(state)) {
              move_down(state);
            }
            state->state = STATE_CONNECT;
            break;

          case Action:
            if (can_rotate(state)) {
              rotate_tetromino(state);
            }
            break;
          default:
            break;
        }
      }
      break;
    default:
      break;
  }
}

GameInfo_t updateCurrentState(GameState *state) {
  if (!state->game_info.pause) {
    switch (state->state) {
      case STATE_START:
        handle_state_start(state);
        break;
      case STATE_SPAWN:
        handle_state_spawn(state);
        break;
      case STATE_MOVE:
        handle_state_move(state);
        break;
      case STATE_SHIFT:
        handle_state_shift(state);
        break;
      case STATE_CONNECT:
        handle_state_connect(state);
        break;
      case STATE_GAME_OVER:
        handle_state_game_over(state);
        break;
    }
    refresh();
  }
  return state->game_info;
}

void handle_state_start(GameState *state) {
  state->last_fall_time = current_timestamp();
}

void handle_state_spawn(GameState *state) {
  state->current_tetromino = state->next_tetromino;

  init_current_tetromino(&state->next_tetromino, rand() % 7);
  state->last_fall_time = current_timestamp();

  if (!can_move_down(state)) {
    state->state = STATE_GAME_OVER;
  } else {
    state->state = STATE_MOVE;
  }
  load_high_score(&state->game_info);
}

void handle_state_move(GameState *state) {
  long long current_time = current_timestamp();
  if (current_time - state->last_fall_time >
      MOVE_DELAY / state->game_info.speed) {
    state->state = STATE_SHIFT;
    state->last_fall_time = current_time;
  }
}

void handle_state_shift(GameState *state) {
  if (can_move_down(state)) {
    move_down(state);
    state->state = STATE_MOVE;
  } else {
    state->state = STATE_CONNECT;
  }
}

void handle_state_connect(GameState *state) {
  fix_tetromino(state);
  int lines_cleared = clear_lines(&state->game_info);
  calculate_score(state, lines_cleared);
  calculate_level_and_speed(state);

  state->state = STATE_SPAWN;
}

void handle_state_game_over(GameState *state) {
  if (state->game_info.score > state->game_info.high_score) {
    state->game_info.high_score = state->game_info.score;
    save_high_score(&state->game_info);
  }
}

GameState *init_game_state(void) {
  GameState *state = (GameState *)malloc(sizeof(GameState));
  init_game_info(&state->game_info);
  init_current_tetromino(&state->current_tetromino, rand() % 7);
  init_current_tetromino(&state->next_tetromino, rand() % 7);
  state->last_fall_time = current_timestamp();
  state->state = STATE_START;

  return state;
}

void destroy_game_state(GameState *state) {
  if (state == NULL) {
    return;
  }
  free_game_info(&state->game_info);
  free(state);
}