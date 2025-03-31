/**
 * @file game_controller.h
 * @brief Заголовочный файл контроллера игры
 * @author lucoscly
 *
 * Этот модуль предоставляет основные функции для управления игровым процессом,
 * включая обработку пользовательского ввода и управление состоянием игры.
 */

#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <stdlib.h>
#include <time.h>

#include "../../../gui/cli/display.h"
#include "../game.h"
#include "../game_state/game_state.h"

/**
 * @brief Обработка пользовательского ввода
 *
 * Преобразует полученный ключ клавиатуры в действие пользователя.
 *
 * @param key Код нажатой клавиши
 * @return UserAction_t Тип действия пользователя
 */
UserAction_t process_user_input(int key);

/**
 * @brief Инициализация новой игры
 *
 * Выделяет память и устанавливает начальное состояние игры.
 *
 * @param state Указатель на указатель GameState, который будет инициализирован
 * @return int Код ошибки (0 при успехе)
 */
int init_game(GameState **state);

/**
 * @brief Запуск основного игрового цикла
 *
 * Управляет игровым процессом от начала до конца или паузы.
 *
 * @param state Указатель на текущее состояние игры
 * @return int Код ошибки (0 при успехе)
 */
int run_game(GameState *state);

/**
 * @brief Очистка ресурсов игры
 *
 * Освобождает всю выделенную память и завершает все активные операции.
 *
 * @param state Указатель на указатель на состояние игры для очистки
 */
void cleanup_game(GameState **state);

#endif