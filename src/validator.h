#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "class.h"  // Подключение заголовочного файла, содержащего класс Stack

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

#endif  // VALIDATOR_H

// 24.11.24 Добавлена читаемость и комментарии