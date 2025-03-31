#include "game.h"

void init_random() { srand(time(NULL)); }

int init_game_info(GameInfo_t *game_info) {
  init_game_parameters(game_info);

  init_game_field(game_info);

  if (init_next_tetromino(game_info) != 0) {
    free_game_field(game_info);
    return -1;
  }

  return 0;
}

void init_game_parameters(GameInfo_t *game_info) {
  game_info->pause = 0;
  game_info->score = 0;
  load_high_score(game_info);
  game_info->speed = 1;
  game_info->level = 1;
  game_info->field = NULL;
  game_info->next = NULL;
}

int init_next_tetromino(GameInfo_t *game_info) {
  game_info->next = (int **)calloc(4, sizeof(int *));
  if (game_info->next == NULL) {
    return -1;
  }

  for (int i = 0; i < 4; i++) {
    game_info->next[i] = NULL;
  }

  for (int i = 0; i < 4; i++) {
    game_info->next[i] = (int *)calloc(4, sizeof(int));
    if (game_info->next[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(game_info->next[j]);
      }
      free(game_info->next);
      game_info->next = NULL;
      return -1;
    }
  }
  return 0;
}

void free_game_info(GameInfo_t *game_info) {
  if (game_info == NULL) return;

  free_game_field(game_info);

  if (game_info->next != NULL) {
    for (int i = 0; i < 4; i++) {
      free(game_info->next[i]);
    }
    free(game_info->next);
    game_info->next = NULL;
  }
}

// Функция для перезапуска игры
int restart_game(GameInfo_t *game_info) {
  free_game_info(game_info);
  return init_game_info(game_info);
}
