/**
 * @file defines.h
 * @brief Основные константы и типы данных для игры Tetris
 * @author lucoscly
 *
 * Этот файл содержит все базовые определения и структуры данных,
 * необходимые для работы игры. Включает размеры поля, константы времени,
 * типы действий пользователя и состояния игры.
 */

#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

#include <time.h>

/**
 * @defgroup game_dimensions Размеры игрового поля
 * @{
 */
#define HEIGHT 20     /**< Высота игрового поля в клетках */
#define WIDTH 10      /**< Ширина игрового поля в клетках */
#define HUD_WIDTH 12  /**< Общая ширина с учетом рамок */
#define BORDER_SIZE 1 /**< Толщина границ игрового поля */
/** @} */             // end of game_dimensions

/**
 * @defgroup timing_params Параметры временных задержек
 * @{
 */
#define REFRESH_DELAY 5 /**< Базовая частота обновления в миллисекундах */
#define MOVE_DELAY 500  /**< Задержка падения первого уровня (мс) */
/** @} */               // end of timing_params

/**
 * @enum UserAction_t
 * @brief Возможные действия пользователя
 */
typedef enum UserAction_t {
  Start,     /**< Запуск новой игры */
  Pause,     /**< Пауза игры */
  Terminate, /**< Завершение игры */
  Left,      /**< Движение влево */
  Right,     /**< Движение вправо */
  Up,        /**< Поворот вверх */
  Down,      /**< Ускоренное падение */
  Action     /**< Действие (поворот) */
} UserAction_t;

/**
 * @enum GameState_t
 * @brief Возможные состояния игры
 */
typedef enum GameState_t {
  STATE_START,    /**< Начальное состояние */
  STATE_SPAWN,    /**< Генерация нового тетрамино */
  STATE_MOVE,     /**< Активное движение тетрамино */
  STATE_SHIFT,    /**< Сдвиг линий */
  STATE_CONNECT,  /**< Проверка соединений */
  STATE_GAME_OVER /**< Конец игры */
} GameState_t;

/**
 * @struct Tetromino_t
 * @brief Структура представления тетрамино
 */
typedef struct Tetromino_t {
  int shape[4][4]; /**< Матрица формы тетрамино (4x4) */
  int x, y;        /**< Текущая позиция на поле */
  int type;        /**< Тип фигуры */
} Tetromino_t;

/**
 * @struct GameInfo_t
 * @brief Структура игровой информации
 */
typedef struct GameInfo_t {
  int **field;    /**< Матрица отвечающая хранящая в себе игровое поле */
  int **next;     /**< Следующая фигура */
  int score;      /**< Игровой счет */
  int high_score; /**< Максимальный счет */
  int level;      /**< Текущий уровень */
  int speed;      /**< Текущая сложность */
  int pause;      /**< Переменная указывающая на то, стоит ли сейчас пауза*/
} GameInfo_t;

/**
 * @struct GameState
 * @brief Структура состояния игры
 */
typedef struct GameState {
  GameInfo_t
      game_info;  // Структура, содержащая базовую информацию о состоянии игры
  Tetromino_t current_tetromino;  // Текущее тетромино
  Tetromino_t next_tetromino;     // Следующее тетромино
  long long last_fall_time;
  GameState_t state;                // Текущая фаза игры
  int field_colors[WIDTH][HEIGHT];  // Массив для хранения цветов тетромино
} GameState;

#endif