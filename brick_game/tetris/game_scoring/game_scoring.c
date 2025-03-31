#include "game_scoring.h"

void calculate_score(GameState *state, int lines_cleared) {
  switch (lines_cleared) {
    case 1:
      state->game_info.score += ONE_LINE_BOUNTY;
      break;
    case 2:
      state->game_info.score += TWO_LINES_BOUNTY;
      break;
    case 3:
      state->game_info.score += THREE_LINES_BOUNTY;
      break;
    case 4:
      state->game_info.score += FOUR_LINES_BOUNTY;
      break;
  }

  if (state->game_info.score > state->game_info.high_score) {
    state->game_info.high_score = state->game_info.score;
    save_high_score(&state->game_info);
  }
}

void calculate_level_and_speed(GameState *state) {
  int new_level = (state->game_info.score / 600) + 1;
  if (new_level > 10) new_level = 10;  // Максимальный уровень - 10

  if (new_level != state->game_info.level) {
    state->game_info.level = new_level;
    state->game_info.speed = new_level;
  }
}

void load_high_score(GameInfo_t *game_info) {
  FILE *f = fopen("highscore.txt", "r");
  if (f) {
    fscanf(f, "%d", &game_info->high_score);
    fclose(f);
  } else {
    game_info->high_score = 0;
  }
}

void save_high_score(GameInfo_t *game_info) {
  FILE *f = fopen("highscore.txt", "w");
  if (f) {
    fprintf(f, "%d", game_info->high_score);
    fclose(f);
  }
}