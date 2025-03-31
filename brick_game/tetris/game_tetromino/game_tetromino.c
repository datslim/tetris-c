#include "game_tetromino.h"

void init_current_tetromino(Tetromino_t *tetromino, int type) {
  const int shapes[7][4][4] = {TETROMINO_I, TETROMINO_O, TETROMINO_T,
                               TETROMINO_S, TETROMINO_Z, TETROMINO_L,
                               TETROMINO_J};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tetromino->shape[i][j] = shapes[type][i][j];
    }
  }
  tetromino->x = WIDTH / 2 - 2;
  tetromino->y = 0;
  tetromino->type = type;
}

int can_move_down(GameState *state) {
  Tetromino_t *tetromino = &state->current_tetromino;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetromino->shape[i][j]) {
        int x = tetromino->x + j;
        int y = tetromino->y + i + 1;
        if (y >= HEIGHT || state->game_info.field[x][y]) {
          return 0;
        }
      }
    }
  }
  return 1;
}

int can_move_left(GameState *state) {
  Tetromino_t *tetromino = &state->current_tetromino;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetromino->shape[i][j]) {
        int x = tetromino->x + j - 1;
        int y = tetromino->y + i;
        if (x < 0 || state->game_info.field[x][y]) {
          return 0;
        }
      }
    }
  }
  return 1;
}

int can_move_right(GameState *state) {
  Tetromino_t *tetromino = &state->current_tetromino;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetromino->shape[i][j]) {
        int x = tetromino->x + j + 1;
        int y = tetromino->y + i;
        if (x >= WIDTH || state->game_info.field[x][y]) {
          return 0;
        }
      }
    }
  }
  return 1;
}

int can_rotate(GameState *state) {
  Tetromino_t *tetromino = &state->current_tetromino;
  int temp[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[i][j] = tetromino->shape[3 - j][i];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (temp[i][j]) {
        int x = tetromino->x + j;
        int y = tetromino->y + i;
        if (x < 0 || x >= WIDTH || y >= HEIGHT ||
            state->game_info.field[x][y]) {
          return 0;
        }
      }
    }
  }
  return 1;
}

void move_left(GameState *state) { state->current_tetromino.x--; }

void move_right(GameState *state) { state->current_tetromino.x++; }

void move_down(GameState *state) { state->current_tetromino.y++; }

void rotate_tetromino(GameState *state) {
  Tetromino_t *tetromino = &state->current_tetromino;
  int temp[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[i][j] = tetromino->shape[3 - j][i];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tetromino->shape[i][j] = temp[i][j];
    }
  }
}

void fix_tetromino(GameState *state) {
  Tetromino_t *tetromino = &state->current_tetromino;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetromino->shape[i][j]) {
        int x = tetromino->x + j;
        int y = tetromino->y + i;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
          state->game_info.field[x][y] = 1;  // помечаем клетку занятой
          state->field_colors[x][y] = tetromino->type;  // сохраняем цвет
        }
      }
    }
  }
}