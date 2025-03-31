#include "game_field.h"

int init_game_field(GameInfo_t *game_info) {
  game_info->field = (int **)calloc(WIDTH, sizeof(int *));
  if (game_info->field == NULL) {
    return -1;
  }

  for (int i = 0; i < WIDTH; i++) {
    game_info->field[i] = NULL;
  }

  for (int i = 0; i < WIDTH; i++) {
    game_info->field[i] = (int *)calloc(HEIGHT, sizeof(int));
    if (game_info->field[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(game_info->field[j]);
      }
      free(game_info->field);
      game_info->field = NULL;
      return -1;
    }
  }
  return 0;
}

int clear_lines(GameInfo_t *game_info) {
  int lines_cleared = 0;
  for (int i = 0; i < HEIGHT; i++) {
    int full = 1;
    for (int j = 0; j < WIDTH; j++) {
      if (!game_info->field[j][i]) {
        full = 0;
        break;
      }
    }
    if (full) {
      lines_cleared++;
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < WIDTH; j++) {
          game_info->field[j][k] = game_info->field[j][k - 1];
        }
      }
      for (int j = 0; j < WIDTH; j++) {
        game_info->field[j][0] = 0;
      }
    }
  }
  return lines_cleared;
}

void free_game_field(GameInfo_t *game_info) {
  if (game_info->field != NULL) {
    for (int i = 0; i < WIDTH; i++) {
      free(game_info->field[i]);
    }
    free(game_info->field);
    game_info->field = NULL;
  }
}