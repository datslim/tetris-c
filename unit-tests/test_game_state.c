#include "test_game_state.h"

#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/game_state/game_state.h"

START_TEST(test_init_game_state) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);
  ck_assert_int_eq(state->state, STATE_START);
  ck_assert_ptr_nonnull(state->current_tetromino.shape);
  ck_assert_ptr_nonnull(state->next_tetromino.shape);
  destroy_game_state(state);
}
END_TEST

START_TEST(test_user_input_start) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Тестируем переход из START в SPAWN
  state->state = STATE_START;
  userInput(state, Start, 0);
  ck_assert_int_eq(state->state, STATE_SPAWN);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_user_input_pause) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Тестируем паузу
  state->state = STATE_MOVE;
  state->game_info.pause = 0;
  userInput(state, Pause, 0);
  ck_assert_int_eq(state->game_info.pause, 1);

  userInput(state, Pause, 0);
  ck_assert_int_eq(state->game_info.pause, 0);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_user_input_movement) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Тестируем движение
  state->state = STATE_MOVE;
  state->game_info.pause = 0;

  // Сохраняем начальную позицию
  int initial_x = state->current_tetromino.x;

  // Тестируем движение влево
  userInput(state, Left, 0);
  ck_assert_int_eq(state->current_tetromino.x, initial_x - 1);

  // Тестируем движение вправо
  userInput(state, Right, 0);
  ck_assert_int_eq(state->current_tetromino.x, initial_x);

  // Тестируем движение вниз
  userInput(state, Down, 0);
  ck_assert_int_eq(
      state->state,
      STATE_CONNECT);  // Проверяем, что состояние изменилось на CONNECT

  destroy_game_state(state);
}
END_TEST

START_TEST(test_handle_state_spawn) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Тестируем спавн новой фигуры
  handle_state_spawn(state);
  ck_assert_int_eq(state->state, STATE_MOVE);
  ck_assert_ptr_nonnull(state->current_tetromino.shape);
  ck_assert_ptr_nonnull(state->next_tetromino.shape);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_handle_state_move) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_MOVE;
  state->last_fall_time = current_timestamp() - MOVE_DELAY - 1;

  handle_state_move(state);
  ck_assert_int_eq(state->state, STATE_SHIFT);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_handle_state_shift) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_SHIFT;

  // Тестируем сдвиг вниз
  handle_state_shift(state);
  ck_assert_int_eq(state->state, STATE_MOVE);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_handle_state_connect) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_CONNECT;
  state->game_info.score = 0;

  handle_state_connect(state);
  ck_assert_int_eq(state->state, STATE_SPAWN);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_handle_state_game_over) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_GAME_OVER;
  state->game_info.score = 1000;
  state->game_info.high_score = 500;

  handle_state_game_over(state);
  ck_assert_int_eq(state->game_info.high_score, 1000);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_update_current_state_start) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_START;
  state->last_fall_time = 0;  // Сбрасываем время

  GameInfo_t info = updateCurrentState(state);
  ck_assert_int_eq(state->state, STATE_START);  // Состояние не меняется
  ck_assert_uint_ne(state->last_fall_time, 0);  // Время обновилось
  ck_assert_int_eq(
      info.score, state->game_info.score);  // Проверяем возвращаемую информацию

  destroy_game_state(state);
}
END_TEST

START_TEST(test_update_current_state_spawn) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_SPAWN;
  Tetromino_t old_next = state->next_tetromino;

  GameInfo_t info = updateCurrentState(state);
  ck_assert_int_eq(state->state, STATE_MOVE);  // Переход в состояние MOVE
  ck_assert_ptr_ne(state->next_tetromino.shape,
                   old_next.shape);  // Новая следующая фигура
  ck_assert_int_eq(
      info.level, state->game_info.level);  // Проверяем возвращаемую информацию

  destroy_game_state(state);
}
END_TEST

START_TEST(test_update_current_state_move) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_MOVE;
  state->last_fall_time =
      current_timestamp() - MOVE_DELAY - 1;  // Прошло достаточно времени

  GameInfo_t info = updateCurrentState(state);
  ck_assert_int_eq(state->state, STATE_SHIFT);  // Переход в состояние SHIFT
  ck_assert_int_eq(
      info.speed, state->game_info.speed);  // Проверяем возвращаемую информацию

  destroy_game_state(state);
}
END_TEST

START_TEST(test_update_current_state_pause) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_MOVE;
  state->game_info.pause = 1;  // Ставим на паузу
  state->last_fall_time = current_timestamp() - MOVE_DELAY - 1;

  GameInfo_t info = updateCurrentState(state);
  ck_assert_int_eq(state->state, STATE_MOVE);  // Состояние не меняется на паузе
  ck_assert_int_eq(info.pause, 1);  // Проверяем, что пауза сохраняется

  destroy_game_state(state);
}
END_TEST

START_TEST(test_update_current_state_game_over) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_GAME_OVER;
  state->game_info.score = 1000;
  state->game_info.high_score = 500;

  GameInfo_t info = updateCurrentState(state);
  ck_assert_int_eq(state->game_info.high_score, 1000);  // Обновился high score
  ck_assert_int_eq(info.high_score, 1000);  // Проверяем возвращаемую информацию

  destroy_game_state(state);
}
END_TEST

START_TEST(test_user_input_game_over) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Тестируем рестарт из GAME_OVER
  state->state = STATE_GAME_OVER;
  state->game_info.score = 1000;
  userInput(state, Start, 0);
  ck_assert_int_eq(state->state, STATE_SPAWN);
  ck_assert_int_eq(state->game_info.score, 0);  // Счет должен обнулиться

  destroy_game_state(state);
}
END_TEST

START_TEST(test_user_input_rotate) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  state->state = STATE_MOVE;
  state->game_info.pause = 0;

  // Устанавливаем фигуру в позицию, где можно повернуть
  state->current_tetromino.x = WIDTH / 2;
  state->current_tetromino.y = 0;

  // Тестируем поворот
  userInput(state, Action, 0);

  // Проверяем, что состояние не изменилось (фигура все еще в движении)
  ck_assert_int_eq(state->state, STATE_MOVE);

  destroy_game_state(state);
}
END_TEST

START_TEST(test_user_input_invalid_state) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Тестируем недопустимое состояние
  state->state = -1;
  int initial_x = state->current_tetromino.x;
  userInput(state, Left, 0);
  ck_assert_int_eq(state->current_tetromino.x,
                   initial_x);  // Позиция не должна измениться

  destroy_game_state(state);
}
END_TEST

START_TEST(test_update_current_state_invalid) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Тестируем недопустимое состояние
  state->state = -1;
  GameInfo_t info = updateCurrentState(state);
  ck_assert_int_eq(
      info.score,
      state->game_info.score);  // Информация должна остаться прежней

  destroy_game_state(state);
}
END_TEST

START_TEST(test_handle_state_spawn_game_over) {
  GameState *state = init_game_state();
  ck_assert_ptr_nonnull(state);

  // Заполняем поле до верха
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      state->game_info.field[i][j] = 1;
    }
  }

  // Тестируем спавн когда нет места
  handle_state_spawn(state);
  ck_assert_int_eq(state->state, STATE_GAME_OVER);

  destroy_game_state(state);
}
END_TEST

Suite *game_state_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Game State");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_game_state);
  tcase_add_test(tc_core, test_user_input_start);
  tcase_add_test(tc_core, test_user_input_pause);
  tcase_add_test(tc_core, test_user_input_movement);
  tcase_add_test(tc_core, test_handle_state_spawn);
  tcase_add_test(tc_core, test_handle_state_move);
  tcase_add_test(tc_core, test_handle_state_shift);
  tcase_add_test(tc_core, test_handle_state_connect);
  tcase_add_test(tc_core, test_handle_state_game_over);
  tcase_add_test(tc_core, test_update_current_state_start);
  tcase_add_test(tc_core, test_update_current_state_spawn);
  tcase_add_test(tc_core, test_update_current_state_move);
  tcase_add_test(tc_core, test_update_current_state_pause);
  tcase_add_test(tc_core, test_update_current_state_game_over);
  tcase_add_test(tc_core, test_user_input_game_over);
  tcase_add_test(tc_core, test_user_input_rotate);
  tcase_add_test(tc_core, test_user_input_invalid_state);
  tcase_add_test(tc_core, test_update_current_state_invalid);
  tcase_add_test(tc_core, test_handle_state_spawn_game_over);

  suite_add_tcase(s, tc_core);

  return s;
}