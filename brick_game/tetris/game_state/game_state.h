/**
 * @file game_state.h
 * @brief Модуль обработки игровых состояний.
 * @author lucoscly
 *
 * Этот модуль предоставляет функции для инициализации, изменения и завершения
 * работы с состояниями игры.
 */

#ifndef TETRIS_STATE_H
#define TETRIS_STATE_H

#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "../../common/defines.h"
#include "../game.h"
#include "../game_scoring/game_scoring.h"

/**
 * @brief Получить текущее время в миллисекундах.
 *
 * @return Текущее время в миллисекундах.
 */
long long current_timestamp();

/**
 * @brief Обработка пользовательского ввода.
 *
 * @param state Указатель на структуру GameState.
 * @param action Действие пользователя.
 * @param hold Флаг, указывающий, удерживается ли клавиша.
 */
void userInput(GameState *state, UserAction_t action, int hold);

/**
 * @brief Обновление текущего состояния игры.
 *
 * @param state Указатель на структуру GameState.
 * @return Информация о текущем состоянии игры.
 */
GameInfo_t updateCurrentState(GameState *state);

// Функции управления состояниями

/**
 * @brief Обработка состояния начала игры.
 *
 * @param state Указатель на структуру GameState.
 */
void handle_state_start(GameState *state);

/**
 * @brief Обработка состояния появления новой фигуры.
 *
 * @param state Указатель на структуру GameState.
 */
void handle_state_spawn(GameState *state);

/**
 * @brief Обработка состояния движения фигуры.
 *
 * @param state Указатель на структуру GameState.
 */
void handle_state_move(GameState *state);

/**
 * @brief Обработка состояния сдвига фигуры.
 *
 * @param state Указатель на структуру GameState.
 */
void handle_state_shift(GameState *state);

/**
 * @brief Обработка состояния соединения фигуры с игровым полем.
 *
 * @param state Указатель на структуру GameState.
 */
void handle_state_connect(GameState *state);

/**
 * @brief Обработка состояния завершения игры.
 *
 * @param state Указатель на структуру GameState.
 */
void handle_state_game_over(GameState *state);

/**
 * @brief Инициализация состояния игры.
 *
 * @return Указатель на структуру GameState.
 */
GameState *init_game_state(void);

/**
 * @brief Освобождение ресурсов, связанных с состоянием игры.
 *
 * @param state Указатель на структуру GameState.
 */
void destroy_game_state(GameState *state);

#endif