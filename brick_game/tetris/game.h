/**
 * @file game.h
 * @brief Основной модуль игры.
 * @author lucoscly
 *
 * Этот модуль предоставляет функции для инициализации, управления и завершения
 * игры.
 */

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../common/defines.h"
#include "game_field/game_field.h"
#include "game_scoring/game_scoring.h"
#include "game_state/game_state.h"
#include "game_tetromino/game_tetromino.h"

/**
 * @brief Инициализация генератора случайных чисел.
 *
 * Эта функция должна быть вызвана перед использованием любых функций, связанных
 * с рандомизацией.
 */
void init_random();

/**
 * @brief Инициализация игровых параметров.
 *
 * @param game_info Указатель на структуру GameInfo_t, которая будет
 * инициализирована.
 */
void init_game_parameters(GameInfo_t *game_info);

/**
 * @brief Инициализация следующего тетромино.
 *
 * @param game_info Указатель на структуру GameInfo_t, содержащую информацию о
 * текущем состоянии игры.
 * @return 0 в случае успеха, -1 в случае ошибки.
 */
int init_next_tetromino(GameInfo_t *game_info);

/**
 * @brief Инициализация информации о игре.
 *
 * @param game_info Указатель на структуру GameInfo_t, которая будет
 * инициализирована.
 * @return 0 в случае успеха, -1 в случае ошибки.
 */
int init_game_info(GameInfo_t *game_info);

/**
 * @brief Освобождение ресурсов, связанных с информацией о игре.
 *
 * @param game_info Указатель на структуру GameInfo_t, ресурсы которой будут
 * освобождены.
 */
void free_game_info(GameInfo_t *game_info);

/**
 * @brief Перезапуск игры.
 *
 * @param game_info Указатель на структуру GameInfo_t, которая будет сброшена в
 * начальное состояние.
 * @return 0 в случае успеха, -1 в случае ошибки.
 */
int restart_game(GameInfo_t *game_info);

#endif  // TETRIS_GAME_H