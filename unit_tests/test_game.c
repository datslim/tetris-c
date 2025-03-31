#define TESTING
#include "test_game.h"

#include <check.h>
#include <stdlib.h>
#include <time.h>

#include "../brick_game/tetris/game.h"
#include "../brick_game/tetris/game_state/game_state.h"
#include "mock_gui.h"

START_TEST(test_init_random) {
  init_random();
  ck_assert_int_eq(1, 1);  // Просто проверка, что тест проходит
}
END_TEST

START_TEST(test_init_game_parameters) {
  GameInfo_t game_info;
  init_game_parameters(&game_info);

  // Проверяем начальные значения
  ck_assert_int_eq(game_info.score, 0);
  ck_assert_int_eq(game_info.level, 1);
  ck_assert_int_eq(game_info.high_score, 0);
  ck_assert_int_eq(game_info.pause, 0);
  ck_assert_int_eq(game_info.speed, 1);
}
END_TEST

START_TEST(test_init_game_info) {
  GameInfo_t game_info;
  int result = init_game_info(&game_info);

  ck_assert_int_eq(result, 0);
  ck_assert_ptr_nonnull(game_info.field);
  ck_assert_ptr_nonnull(game_info.next);

  // Очищаем ресурсы
  free_game_info(&game_info);
}
END_TEST

START_TEST(test_restart_game) {
  GameInfo_t game_info;
  init_game_info(&game_info);

  // Изменяем некоторые значения
  game_info.score = 100;
  game_info.level = 2;

  int result = restart_game(&game_info);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(game_info.score, 0);
  ck_assert_int_eq(game_info.level, 1);

  // Очищаем ресурсы
  free_game_info(&game_info);
}
END_TEST

START_TEST(test_free_game_info_null) {
  free_game_info(NULL);

  ck_assert_int_eq(1, 1);
}
END_TEST

Suite *game_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Game");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_game_parameters);
  tcase_add_test(tc_core, test_init_game_info);
  tcase_add_test(tc_core, test_restart_game);
  tcase_add_test(tc_core, test_free_game_info_null);

  suite_add_tcase(s, tc_core);

  return s;
}