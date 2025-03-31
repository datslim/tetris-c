#include "test_game_field.h"

#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/game_field/game_field.h"
#include "../brick_game/tetris/game_state/game_state.h"

START_TEST(test_init_game_field) {
  GameInfo_t game_info;
  int result = init_game_field(&game_info);

  ck_assert_int_eq(result, 0);
  ck_assert_ptr_nonnull(game_info.field);

  // Проверяем, что поле инициализировано нулями
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      ck_assert_int_eq(game_info.field[i][j], 0);
    }
  }

  free_game_field(&game_info);
}
END_TEST

START_TEST(test_clear_lines) {
  GameInfo_t game_info;
  init_game_field(&game_info);

  // Заполняем нижнюю строку
  for (int i = 0; i < WIDTH; i++) {
    game_info.field[i][HEIGHT - 1] = 1;
  }

  int lines_cleared = clear_lines(&game_info);
  ck_assert_int_eq(lines_cleared, 1);

  // Проверяем, что строка очищена
  for (int i = 0; i < WIDTH; i++) {
    ck_assert_int_eq(game_info.field[i][HEIGHT - 1], 0);
  }

  free_game_field(&game_info);
}
END_TEST

START_TEST(test_clear_multiple_lines) {
  GameInfo_t game_info;
  init_game_field(&game_info);

  // Заполняем две нижние строки
  for (int i = 0; i < WIDTH; i++) {
    game_info.field[i][HEIGHT - 1] = 1;
    game_info.field[i][HEIGHT - 2] = 1;
  }

  int lines_cleared = clear_lines(&game_info);
  ck_assert_int_eq(lines_cleared, 2);

  // Проверяем, что строки очищены
  for (int i = 0; i < WIDTH; i++) {
    ck_assert_int_eq(game_info.field[i][HEIGHT - 1], 0);
    ck_assert_int_eq(game_info.field[i][HEIGHT - 2], 0);
  }

  free_game_field(&game_info);
}
END_TEST

START_TEST(test_clear_lines_with_gaps) {
  GameInfo_t game_info;
  init_game_field(&game_info);

  // Заполняем нижнюю строку с пропуском
  for (int i = 0; i < WIDTH; i++) {
    if (i != WIDTH / 2) {  // Оставляем пустую клетку в середине
      game_info.field[i][HEIGHT - 1] = 1;
    }
  }

  int lines_cleared = clear_lines(&game_info);
  ck_assert_int_eq(lines_cleared, 0);  // Линия не должна очиститься

  // Проверяем, что строка осталась без изменений
  for (int i = 0; i < WIDTH; i++) {
    if (i != WIDTH / 2) {
      ck_assert_int_eq(game_info.field[i][HEIGHT - 1], 1);
    } else {
      ck_assert_int_eq(game_info.field[i][HEIGHT - 1], 0);
    }
  }

  free_game_field(&game_info);
}
END_TEST

START_TEST(test_clear_lines_shift) {
  GameInfo_t game_info;
  init_game_field(&game_info);

  // Заполняем только нижнюю строку
  for (int i = 0; i < WIDTH; i++) {
    game_info.field[i][HEIGHT - 1] = 1;  // Нижняя строка
  }

  // Очищаем нижнюю строку
  int lines_cleared = clear_lines(&game_info);
  ck_assert_int_eq(lines_cleared, 1);

  // Проверяем, что строка очищена
  for (int i = 0; i < WIDTH; i++) {
    ck_assert_int_eq(game_info.field[i][HEIGHT - 1], 0);
  }

  free_game_field(&game_info);
}
END_TEST

START_TEST(test_free_game_field) {
  GameInfo_t game_info;
  init_game_field(&game_info);

  // Проверяем, что поле не NULL
  ck_assert_ptr_nonnull(game_info.field);

  // Освобождаем память
  free_game_field(&game_info);

  // Проверяем, что поле стало NULL
  ck_assert_ptr_null(game_info.field);
}
END_TEST

START_TEST(test_free_game_field_null) {
  // Тест освобождения NULL поля
  GameInfo_t game_info = {0};
  free_game_field(&game_info);
  ck_assert_ptr_null(game_info.field);
}
END_TEST

Suite *game_field_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Game Field");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_game_field);
  tcase_add_test(tc_core, test_clear_lines);
  tcase_add_test(tc_core, test_clear_multiple_lines);
  tcase_add_test(tc_core, test_clear_lines_with_gaps);
  tcase_add_test(tc_core, test_clear_lines_shift);
  tcase_add_test(tc_core, test_free_game_field);
  tcase_add_test(tc_core, test_free_game_field_null);

  suite_add_tcase(s, tc_core);

  return s;
}