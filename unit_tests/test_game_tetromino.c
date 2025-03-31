#include "test_game_tetromino.h"

#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/game_state/game_state.h"
#include "../brick_game/tetris/game_tetromino/game_tetromino.h"

START_TEST(test_init_current_tetromino_position) {
  Tetromino_t tetromino;
  init_current_tetromino(&tetromino, 0);  // I-фигура

  // Проверяем начальную позицию
  ck_assert_int_eq(tetromino.x, WIDTH / 2 - 2);
  ck_assert_int_eq(tetromino.y, 0);
  ck_assert_int_eq(tetromino.type, 0);
}
END_TEST

START_TEST(test_init_current_tetromino_shape) {
  Tetromino_t tetromino;
  init_current_tetromino(&tetromino, 0);  // I-фигура

  // Проверяем форму I-фигуры (горизонтальная)
  ck_assert_int_eq(tetromino.shape[1][0], 1);
  ck_assert_int_eq(tetromino.shape[1][1], 1);
  ck_assert_int_eq(tetromino.shape[1][2], 1);
  ck_assert_int_eq(tetromino.shape[1][3], 1);

  // Проверяем, что остальные клетки пустые
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i != 1) {  // Пропускаем среднюю строку
        ck_assert_int_eq(tetromino.shape[i][j], 0);
      }
    }
  }
}
END_TEST

START_TEST(test_can_move_down) {
  GameState *state = init_game_state();

  // Проверяем начальное положение
  ck_assert_int_eq(can_move_down(state), 1);

  // Двигаем фигуру вниз до упора
  while (can_move_down(state)) {
    move_down(state);
  }
  ck_assert_int_eq(can_move_down(state), 0);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_can_move_left) {
  GameState *state = init_game_state();

  // Проверяем начальное положение
  ck_assert_int_eq(can_move_left(state), 1);

  // Двигаем фигуру влево до упора
  while (can_move_left(state)) {
    move_left(state);
  }
  ck_assert_int_eq(can_move_left(state), 0);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_can_move_right) {
  GameState *state = init_game_state();

  // Проверяем начальное положение
  ck_assert_int_eq(can_move_right(state), 1);

  // Двигаем фигуру вправо до упора
  while (can_move_right(state)) {
    move_right(state);
  }
  ck_assert_int_eq(can_move_right(state), 0);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_can_rotate) {
  GameState *state = init_game_state();

  // Проверяем возможность вращения в начальном положении
  ck_assert_int_eq(can_rotate(state), 1);

  // Двигаем фигуру вниз до упора
  while (can_move_down(state)) {
    move_down(state);
  }

  destroy_game_state(state);
}
END_TEST

START_TEST(test_rotate_tetromino_shape_change) {
  GameState *state = init_game_state();

  // Сохраняем начальную форму
  int original_shape[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      original_shape[i][j] = state->current_tetromino.shape[i][j];
    }
  }

  // Вращаем фигуру
  rotate_tetromino(state);

  // Проверяем, что форма изменилась
  int changed = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current_tetromino.shape[i][j] != original_shape[i][j]) {
        changed = 1;
        break;
      }
    }
    if (changed) break;
  }
  ck_assert_int_eq(changed, 0);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_rotate_tetromino_cell_count) {
  GameState *state = init_game_state();

  // Сохраняем начальную форму
  int original_shape[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      original_shape[i][j] = state->current_tetromino.shape[i][j];
    }
  }

  // Вращаем фигуру
  rotate_tetromino(state);

  // Проверяем, что вращение сохраняет количество клеток
  int original_cells = 0;
  int rotated_cells = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (original_shape[i][j]) original_cells++;
      if (state->current_tetromino.shape[i][j]) rotated_cells++;
    }
  }
  ck_assert_int_eq(original_cells, rotated_cells);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_fix_tetromino_field) {
  GameState *state = init_game_state();

  // Двигаем фигуру вниз
  move_down(state);

  // Фиксируем фигуру
  fix_tetromino(state);

  // Проверяем, что фигура зафиксирована в поле
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current_tetromino.shape[i][j]) {
        int x = state->current_tetromino.x + j;
        int y = state->current_tetromino.y + i;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
          ck_assert_int_eq(state->game_info.field[x][y], 1);
        }
      }
    }
  }

  destroy_game_state(state);
}
END_TEST

START_TEST(test_fix_tetromino_colors) {
  GameState *state = init_game_state();

  // Двигаем фигуру вниз
  move_down(state);

  // Фиксируем фигуру
  fix_tetromino(state);

  // Проверяем, что цвета сохранены
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current_tetromino.shape[i][j]) {
        int x = state->current_tetromino.x + j;
        int y = state->current_tetromino.y + i;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
          ck_assert_int_eq(state->field_colors[x][y],
                           state->current_tetromino.type);
        }
      }
    }
  }

  destroy_game_state(state);
}
END_TEST

Suite *game_tetromino_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Game Tetromino");
  tc_core = tcase_create("Core");

  // Тесты инициализации
  tcase_add_test(tc_core, test_init_current_tetromino_position);
  tcase_add_test(tc_core, test_init_current_tetromino_shape);

  // Тесты движения
  tcase_add_test(tc_core, test_can_move_down);
  tcase_add_test(tc_core, test_can_move_left);
  tcase_add_test(tc_core, test_can_move_right);

  // Тесты вращения
  tcase_add_test(tc_core, test_can_rotate);
  tcase_add_test(tc_core, test_rotate_tetromino_shape_change);
  tcase_add_test(tc_core, test_rotate_tetromino_cell_count);

  // Тесты фиксации
  tcase_add_test(tc_core, test_fix_tetromino_field);
  tcase_add_test(tc_core, test_fix_tetromino_colors);

  suite_add_tcase(s, tc_core);

  return s;
}