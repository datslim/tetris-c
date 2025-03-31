#include "test_game_scoring.h"

#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/game_scoring/game_scoring.h"
#include "../brick_game/tetris/game_state/game_state.h"

START_TEST(test_calculate_score) {
  GameState *state = init_game_state();

  // Тестируем начисление очков за разное количество линий
  calculate_score(state, 1);
  ck_assert_int_eq(state->game_info.score, ONE_LINE_BOUNTY);

  calculate_score(state, 2);
  ck_assert_int_eq(state->game_info.score, ONE_LINE_BOUNTY + TWO_LINES_BOUNTY);

  calculate_score(state, 3);
  ck_assert_int_eq(state->game_info.score,
                   ONE_LINE_BOUNTY + TWO_LINES_BOUNTY + THREE_LINES_BOUNTY);

  calculate_score(state, 4);
  ck_assert_int_eq(state->game_info.score, ONE_LINE_BOUNTY + TWO_LINES_BOUNTY +
                                               THREE_LINES_BOUNTY +
                                               FOUR_LINES_BOUNTY);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_calculate_level_and_speed) {
  GameState *state = init_game_state();

  // Проверяем начальный уровень
  ck_assert_int_eq(state->game_info.level, 1);
  ck_assert_int_eq(state->game_info.speed, 1);

  // Проверяем уровень 2 (600 очков)
  state->game_info.score = 600;
  calculate_level_and_speed(state);
  ck_assert_int_eq(state->game_info.level, 2);
  ck_assert_int_eq(state->game_info.speed, 2);

  // Проверяем уровень 3 (1200 очков)
  state->game_info.score = 1200;
  calculate_level_and_speed(state);
  ck_assert_int_eq(state->game_info.level, 3);
  ck_assert_int_eq(state->game_info.speed, 3);

  // Проверяем максимальный уровень (6000 очков)
  state->game_info.score = 6000;
  calculate_level_and_speed(state);
  ck_assert_int_eq(state->game_info.level, 10);
  ck_assert_int_eq(state->game_info.speed, 10);

  // Проверяем, что уровень не превышает 10 (10000 очков)
  state->game_info.score = 10000;
  calculate_level_and_speed(state);
  ck_assert_int_eq(state->game_info.level, 10);
  ck_assert_int_eq(state->game_info.speed, 10);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_high_score) {
  GameState *state = init_game_state();

  // Тестируем сохранение и загрузку рекорда
  state->game_info.score = 1000;
  state->game_info.high_score = 500;
  calculate_score(state, 0);  // Проверяет и обновляет рекорд

  ck_assert_int_eq(state->game_info.high_score, 1000);

  // Создаем новое состояние и проверяем загрузку рекорда
  GameState *new_state = init_game_state();
  load_high_score(&new_state->game_info);
  ck_assert_int_eq(new_state->game_info.high_score, 1000);

  destroy_game_state(state);
  destroy_game_state(new_state);
}
END_TEST

Suite *game_scoring_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Game Scoring");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_calculate_score);
  tcase_add_test(tc_core, test_calculate_level_and_speed);
  tcase_add_test(tc_core, test_high_score);

  suite_add_tcase(s, tc_core);

  return s;
}