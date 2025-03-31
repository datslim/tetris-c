#include <check.h>
#include <stdlib.h>

#include "test_game.h"
#include "test_game_field.h"
#include "test_game_scoring.h"
#include "test_game_state.h"
#include "test_game_tetromino.h"

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = game_state_suite();
  sr = srunner_create(s);

  srunner_add_suite(sr, game_field_suite());
  srunner_add_suite(sr, game_scoring_suite());
  srunner_add_suite(sr, game_tetromino_suite());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}