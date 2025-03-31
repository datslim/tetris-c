/**
 * @file game_field.h
 * @brief Заголовочный файл для управления игровым полем
 * @author lucoscly
 *
 * Этот модуль предоставляет функции для инициализации, очистки и освобождения
 * памяти игрового поля. Включает обработку заполненных линий и управление
 * состоянием игровой области.
 */

#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <stdlib.h>

#include "../game.h"

/**
 * @brief Инициализация игрового поля
 *
 * Выделяет память под игровое поле и устанавливает начальное состояние.
 * Создает двумерный массив для основного поля и следующего тетрамино.
 *
 * @param game_info Указатель на структуру GameInfo_t для инициализации
 * @return int Код ошибки (0 при успехе, отрицательное значение при ошибке)
 */
int init_game_field(GameInfo_t *game_info);

/**
 * @brief Очистка заполненных линий
 *
 * Проверяет все строки поля на заполненность и удаляет полные линии.
 * Сдвигает оставшиеся линии вниз и обновляет счетчик очков.
 *
 * @param game_info Указатель на текущую информацию о игре
 * @return int Количество удаленных линий (0 если линий не было удалено)
 */
int clear_lines(GameInfo_t *game_info);

/**
 * @brief Освобождение памяти игрового поля
 *
 * Освобождает всю выделенную под игровое поле память.
 * Должен быть вызван перед завершением работы программы.
 *
 * @param game_info Указатель на структуру GameInfo_t для очистки
 */
void free_game_field(GameInfo_t *game_info);

#endif  // GAME_FIELD_H