// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <cctype>   // Для работы с функциями проверки символов, например, isdigit
#include <cmath>    // Для математических функций, например, sin, cos, exp
#include <iostream> // Для ввода и вывода
#include <limits>   // Для работы с пределами значений, например, std::numeric_limits
#include <locale>   // Для работы с локалями
#include <regex>    // Для регулярных выражений
#include <stack>    // Для использования стека
#include <string>   // Для работы со строками
#include <utility>  // Для std::pair
#include <vector>   // Для использования контейнера vector

#include "class.h" // Подключение заголовочного файла, содержащего класс Stack

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

#define SHIFT 0.01         // Сдвиг для численного расчета
#define LEFT_BORDER -5    // Левая граница диапазона расчета
#define RIGHT_BORDER 5    // Правая граница диапазона расчета

using namespace std;

// parser.cpp
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

// calculation.cpp

// Функция для выполнения вычислений на основе выходного массива
double calculator(vector<vector<double>> &output_arr, unsigned int &i_out_arr, double &coordinate_x);

// Функция для расчета координат на основе интервалов
void calculating_coordinate(vector<vector<double>> &output_arr, unsigned int &i_out_arr,
                            vector<vector<double>> &coordinate_arr, vector<pair<double, double>> intervals);

// Функция для печати массива координат
void print_coordinate_arr(vector<vector<double>> &coordinate_arr);

// validator.cpp

// Функция для проверки правильности использования скобок
bool checkBrackets(const std::string &expression, std::string &error);

// Функция для удаления пробелов из строки
std::string removeSpaces(const std::string &str);

// Функция для проверки валидных символов
bool checkValidCharacters(const std::string &expression, std::string &error);

// Функция для проверки правильного использования операторов
bool checkOperators(const std::string &expression, std::string &error);

// Функция для проверки наличия операндов в строке
bool checkOperands(const std::string &expression, std::string &error);

// Основная функция-валидатор для проверки математического выражения
bool validateMathExpression(const std::string &expression);

// Функция для проверки корректности имен функций в выражении
bool checkFunctionNames(const std::string &expression, std::string &error);

// scope_of_definition.cpp

// Функция для печати интервалов
void print_intervals(vector<pair<double, double>> &intervals);

// Функция для проверки наличия NaN значений в диапазоне
void check_nan_by_range(vector<vector<double>> &output_arr, unsigned int &i_out_arr, vector<bool> &nan_arr);

// Функция для создания интервалов на основе массива NaN значений
void create_intervals(vector<bool> &nan_arr, vector<pair<double, double>> &intervals);

// break_points.cpp

// Функция для поиска точек разрыва
void search_break_points(vector<pair<double, double>> &intervals, vector<pair<double, int>> &break_points,
                         vector<vector<double>> &output_arr, unsigned int &i_out_arr);

// Функция для печати точек разрыва
void print_break_points(vector<pair<double, int>> &break_points);

// asymptote.cpp

// Функция для поиска вертикальной асимптоты
void search_vertical_asymptote(vector<pair<double, double>> &intervals, vector<vector<double>> &output_arr,
                               unsigned int &i_out_arr, vector<double> &vertical_asymptote);

// Функция для печати вертикальной асимптоты
void print_vertical_asymptote(vector<double> &vertical_asymptote);

// Функция для поиска горизонтальной асимптоты
pair<double, double> search_horizontal_asymptote(vector<pair<double, double>> &intervals,
                                                 vector<vector<double>> &output_arr,
                                                 unsigned int &i_out_arr);
// Функция проверки вхождения тангенса
bool tg_check(const vector<vector<double>> &output_arr);

#endif  // PARSER_H

//07.11.24 Изменена структура + добавлены коменнтарии для улучшения читаемости кода