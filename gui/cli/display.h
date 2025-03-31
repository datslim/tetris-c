/**
 * @file display.h
 * @brief Модуль для отображения игрового интерфейса в терминале.
 * @author lucoscly
 *
 * Этот модуль предоставляет функции для инициализации, отрисовки и завершения
 * работы с библиотекой ncurses.
 */

#ifndef CLI_DISPLAY_H
#define CLI_DISPLAY_H

#include <ncurses.h>

#include "../../brick_game/common/defines.h"

/**
 * @brief Рисует прямоугольник с заданными параметрами.
 *
 * @param y Координата Y верхнего левого угла прямоугольника.
 * @param x Координата X верхнего левого угла прямоугольника.
 * @param height Высота прямоугольника.
 * @param width Ширина прямоугольника.
 */
void draw_box(int y, int x, int height, int width);

/**
 * @brief Инициализирует библиотеку ncurses.
 */
void init_ncurses(void);

/**
 * @brief Инициализирует цветовые пары для ncurses.
 */
void init_colors(void);

/**
 * @brief Освобождает ресурсы, связанные с библиотекой ncurses.
 */
void cleanup_ncurses(void);

/**
 * @brief Отображает стартовое сообщение.
 */
void print_start_message(void);

/**
 * @brief Отображает сообщение о паузе.
 */
void print_pause(void);

/**
 * @brief Отображает сообщение о завершении игры.
 */
void print_game_over(void);

/**
 * @brief Отображает информацию о текущем состоянии игры.
 *
 * @param state Указатель на структуру GameState, содержащую текущее состояние
 * игры.
 */
void print_game_info(GameState *state);

/**
 * @brief Отображает игровое поле.
 */
void print_field(void);

/**
 * @brief Отображает зафиксированные тетромино на игровом поле.
 *
 * @param state Указатель на структуру GameState, содержащую текущее состояние
 * игры.
 */
void print_fixed_tetronimos(GameState *state);

/**
 * @brief Отображает текущее движущееся тетромино.
 *
 * @param tetromino Указатель на структуру Tetromino_t, представляющую текущее
 * тетромино.
 */
void print_moving_tetronimo(Tetromino_t *tetromino);

/**
 * @brief Отображает следующее тетромино.
 *
 * @param next_tetromino Указатель на структуру Tetromino_t, представляющую
 * следующее тетромино.
 */
void print_next_tetromino(Tetromino_t *next_tetromino);

/**
 * @brief Отображает статистику игры.
 *
 * @param stats Указатель на структуру GameInfo_t, содержащую статистику игры.
 */
void print_stats(GameInfo_t *stats);

/**
 * @brief Отображает информацию об управлении в игре.
 */
void print_controls(void);

#endif  // CLI_DISPLAY_H