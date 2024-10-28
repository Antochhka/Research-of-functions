// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <cctype>
#include <iostream>
#include <locale>
#include <regex>
#include <stack>
#include <string>
#include <vector>

#include "class.h"

enum TokenType {
    BUFFER = 100,
    LEFT_BORDER = -1,
    RIGHT_BORDER = 1,
    Y = 0,
    NONE = 0,
    CODIF = 0,
    DATA = 1,
    NUM = 0,
    X = 1,
    PLUS = 2,
    MINUS = 3,
    MULT = 4,
    DIV = 5,
    SIN = 6,
    COS = 7,
    TG = 8,
    ASIN = 9,
    ACOS = 10,
    ATG = 11,
    SQRT = 12,
    EXP = 13,
    LOG = 14,
    LOG10 = 15,
    POW = 16,
    UMINUS = 17
};

#define SHIFT 0.1

using namespace std;

// parser.cpp
void parser(vector<vector<double>> &output_arr, unsigned int &i_out_arr, string &input);
void print_arr(vector<vector<double>> &output_arr, unsigned int &i_out_arr);
unsigned int is_digit(unsigned int &i, string &input, double &sum, vector<vector<double>> &output_arr,
                      unsigned int &i_out_arr);
unsigned int priority(char el);
unsigned int codifications(char el);
void stack_in_output_arr(Stack<char> &operations_stack, vector<vector<double>> &output_arr,
                         unsigned int &i_out_arr);
unsigned int is_func(unsigned int &i, string &input, char &result);

// calculation.cpp
double calculator(vector<vector<double>> &output_arr, unsigned int &i_out_arr, double &coordinate_x);
void calculating_coordinate(vector<vector<double>> &output_arr, unsigned int &i_out_arr,
                            vector<vector<double>> &coordinate_arr);

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

// Основная функция-валидатор
bool validateMathExpression(const std::string &expression);

bool checkFunctionNames(const std::string& expression, std::string& error);


#endif  // PARSER_H