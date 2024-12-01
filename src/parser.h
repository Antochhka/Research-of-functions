// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <cctype>  // Для работы с функциями проверки символов, например, isdigit
#include <cmath>     // Для математических функций, например, sin, cos, exp
#include <iostream>  // Для ввода и вывода
#include <limits>  // Для работы с пределами значений, например, std::numeric_limits
#include <locale>   // Для работы с локалями
#include <regex>    // Для регулярных выражений
#include <stack>    // Для использования стека
#include <string>   // Для работы со строками
#include <utility>  // Для std::pair
#include <vector>   // Для использования контейнера vector

#include "class.h"  // Подключение заголовочного файла, содержащего класс Stack

// Перечисление TokenType для определения различных типов токенов в анализаторе
enum TokenType {
    BUFFER = 100,  // Буфер
    BP_1 = 1,      // Устранимая точка перегиба 1 рода
    DBP_1 = 2,     // Неустранимая точка перегиба 1 рода
    BP_2 = 3,      // Точка перегиба 2 рода
    Y = 0,         // Значение функции
    NONE = 0,      // Отсутствие значения
    CODIF = 0,     // Кодировка токена
    DATA = 1,      // Данные токена
    NUM = 0,       // Число
    X = 1,         // Переменная X
    PLUS = 2,      // Плюс
    MINUS = 3,     // Минус
    MULT = 4,      // Умножение
    DIV = 5,       // Деление
    SIN = 6,       // Синус
    COS = 7,       // Косинус
    TG = 8,        // Тангенс
    ASIN = 9,      // Арксинус
    ACOS = 10,     // Арккосинус
    ATG = 11,      // Арктангенс
    SQRT = 12,     // Квадратный корень
    EXP = 13,      // Экспонента
    LOG = 14,      // Логарифм
    LOG10 = 15,    // Десятичный логарифм
    POW = 16,      // Возведение в степень
    UMINUS = 17    // Унарный минус
};

#define SHIFT 0.01      // Сдвиг для численного расчета
#define LEFT_BORDER -3  // Левая граница диапазона расчета
#define RIGHT_BORDER 3  // Правая граница диапазона расчета

using namespace std;

// Функция для разбора входного выражения
void parser(vector<vector<double>> &output_arr, unsigned int &i_out_arr, string &input);

// Функция для печати содержимого выходного массива
void print_arr(vector<vector<double>> &output_arr, unsigned int &i_out_arr);

// Функция для обработки чисел в строке и их добавления в выходной массив
unsigned int is_digit(unsigned int &i, string &input, double &sum, vector<vector<double>> &output_arr,
                      unsigned int &i_out_arr);

// Функция для определения приоритета операторов
unsigned int priority(char el);

// Функция для получения кодировки оператора или функции
unsigned int codifications(char el);

// Перенос всех оставшихся операторов из стека в выходной массив
void stack_in_output_arr(Stack<char> &operations_stack, vector<vector<double>> &output_arr,
                         unsigned int &i_out_arr);

// Проверка на наличие функции (например, sin, cos) в текущей позиции строки
unsigned int is_func(unsigned int &i, string &input, char &result);

#endif  // PARSER_H

// 24.11.24 Изменена структура + добавлены комментарии для улучшения читаемости
// кода
