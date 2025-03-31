#include "display.h"

void draw_box(int y, int x, int height, int width) {
  mvaddch(y, x, ACS_ULCORNER);
  mvaddch(y, x + width - 1, ACS_URCORNER);
  mvaddch(y + height - 1, x, ACS_LLCORNER);
  mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);

  for (int i = 1; i < width - 1; i++) {
    mvaddch(y, x + i, ACS_HLINE);
    mvaddch(y + height - 1, x + i, ACS_HLINE);
  }

  for (int i = 1; i < height - 1; i++) {
    mvaddch(y + i, x, ACS_VLINE);
    mvaddch(y + i, x + width - 1, ACS_VLINE);
  }
}

void init_ncurses(void) {
  initscr();
  start_color();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  timeout(0);
  init_colors();
}

void cleanup_ncurses(void) {
  clear();
  refresh();
  endwin();
  delscreen(NULL);
}

void init_colors(void) {
  init_pair(1, COLOR_CYAN, COLOR_CYAN);        // I-фигура
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);    // O-фигура
  init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);  // T-фигура
  init_pair(4, COLOR_GREEN, COLOR_GREEN);      // S-фигура
  init_pair(5, COLOR_RED, COLOR_RED);          // Z-фигура
  init_pair(6, COLOR_BLUE, COLOR_BLUE);        // L-фигура
  init_pair(7, COLOR_WHITE, COLOR_WHITE);      // J-фигура
  init_pair(8, COLOR_WHITE, COLOR_BLACK);      // Белая рамка на черном фоне
  init_pair(9, COLOR_GREEN, COLOR_BLACK);      // Информация о старте
  init_pair(10, COLOR_RED, COLOR_BLACK);       // Красный текст для проигрыша
  init_pair(11, COLOR_MAGENTA, COLOR_BLACK);   // Фиолетовый текст для паузы
}

void print_start_message(void) {
  attron(COLOR_PAIR(9) | A_BOLD);
  mvprintw(HEIGHT + 2, 2, "Press SPACE to Start");
  attroff(COLOR_PAIR(9) | A_BOLD);
}

void print_pause(void) {
  attron(COLOR_PAIR(11) | A_BOLD);
  mvprintw(HEIGHT + 2, 2, "PAUSED - Press P to Continue");
  attroff(COLOR_PAIR(11) | A_BOLD);
}

void print_game_over(void) {
  attron(COLOR_PAIR(10) | A_BOLD);
  mvprintw(HEIGHT + 2, 2, "GAME OVER - Press SPACE to Start New Game");
  attroff(COLOR_PAIR(10) | A_BOLD);
}

void print_game_info(GameState *state) {
  erase();
  print_field();
  print_fixed_tetronimos(state);
  print_moving_tetronimo(&state->current_tetromino);
  print_next_tetromino(&state->next_tetromino);
  print_stats(&state->game_info);
  print_controls();

  if (state->state == STATE_START) {
    print_start_message();
  } else if (state->state == STATE_GAME_OVER) {
    print_game_over();
  } else if (state->game_info.pause) {
    print_pause();
  }

  refresh();
}

void print_field(void) {
  attron(COLOR_PAIR(8));
  for (int i = 1; i < WIDTH * 2 + 3; i++) {
    if (i == 1) {
      mvaddch(0, i, ACS_ULCORNER);  // Верхний левый угол
    } else if (i == WIDTH * 2 + 2) {
      mvaddch(0, i, ACS_URCORNER);  // Верхний правый угол
    } else {
      mvaddch(0, i, ACS_HLINE);  // Верхняя граница
    }
  }
  attroff(COLOR_PAIR(8));

  for (int i = 1; i < HEIGHT + 1; i++) {
    mvaddch(i, 1, ACS_VLINE);              // Левая граница
    mvaddch(i, WIDTH * 2 + 2, ACS_VLINE);  // Правая граница
  }

  attron(COLOR_PAIR(8));
  for (int i = 1; i < WIDTH * 2 + 3; i++) {
    if (i == 1) {
      mvaddch(HEIGHT + 1, i, ACS_LLCORNER);  // Нижний левый угол
    } else if (i == WIDTH * 2 + 2) {
      mvaddch(HEIGHT + 1, i, ACS_LRCORNER);  // Нижний правый угол
    } else {
      mvaddch(HEIGHT + 1, i, ACS_HLINE);  // Нижняя граница
    }
  }
  attroff(COLOR_PAIR(8));
}

void print_fixed_tetronimos(GameState *state) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (state->game_info.field[j][i]) {
        int type = state->field_colors[j][i];
        attron(COLOR_PAIR(type + 1));  // Используем цвет, соответствующий типу
        move(i + BORDER_SIZE, j * 2 + BORDER_SIZE * 2);
        printw("  ");
        attroff(COLOR_PAIR(type + 1));
      }
    }
  }
}

void print_moving_tetronimo(Tetromino_t *tetromino) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetromino->shape[i][j]) {
        int x = tetromino->x + j;
        int y = tetromino->y + i;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
          move(y + BORDER_SIZE, x * 2 + BORDER_SIZE * 2);
          attron(COLOR_PAIR(tetromino->type + 1));
          printw("  ");
          attroff(COLOR_PAIR(tetromino->type + 1));
        }
      }
    }
  }
}

void print_next_tetromino(Tetromino_t *next_tetromino) {
  int start_x = WIDTH * 2 + 4;
  int start_y = 0;
  int box_width = 22;
  int box_height = 7;

  // Рамка
  attron(COLOR_PAIR(8));
  draw_box(start_y, start_x, box_height, box_width);
  attroff(COLOR_PAIR(8));
  // Надпись
  mvprintw(start_y + 1, start_x + (box_width - 5) / 2, "NEXT:");

  // Фигура
  int tetro_start_x = start_x + (box_width - 8) / 2;
  int tetro_start_y = start_y + 2;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (next_tetromino->shape[i][j]) {
        move(tetro_start_y + i, tetro_start_x + j * 2);
        attron(COLOR_PAIR(next_tetromino->type + 1));
        printw("  ");
        attroff(COLOR_PAIR(next_tetromino->type + 1));
      }
    }
  }
}

void print_stats(GameInfo_t *stats) {
  int start_x = WIDTH * 2 + 4;
  int start_y = 7;
  int box_width = 22;
  int box_height = 5;

  // Рамка
  attron(COLOR_PAIR(8));
  draw_box(start_y, start_x, box_height, box_width);
  attroff(COLOR_PAIR(8));

  // Надписи
  mvprintw(start_y + 1, start_x + ((box_width - 7) / 2), "LEVEL: %-4d",
           stats->level);
  mvprintw(start_y + 2, start_x + ((box_width - 7) / 2), "SCORE: %-4d",
           stats->score);
  mvprintw(start_y + 3, start_x + ((box_width - 7) / 2), "HIGH : %-4d",
           stats->high_score);
}

void print_controls(void) {
  int start_x = WIDTH * 2 + 4;
  int start_y = 12;
  int box_width = 22;
  int box_height = 10;

  attron(COLOR_PAIR(8));
  draw_box(start_y, start_x, box_height, box_width);
  attroff(COLOR_PAIR(8));

  mvprintw(start_y + 1, start_x + (box_width - 8) / 2, "Controls");

  mvprintw(start_y + 3, start_x + 2, "LEFT/RIGHT: Move");
  mvprintw(start_y + 4, start_x + 2, "ENTER     : Rotate");
  mvprintw(start_y + 5, start_x + 2, "DOWN      : Drop");
  mvprintw(start_y + 6, start_x + 2, "P         : Pause");
  mvprintw(start_y + 7, start_x + 2, "Q         : Quit");
  mvprintw(start_y + 8, start_x + 2, "SPACE     : Start");
}
