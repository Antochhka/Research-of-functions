// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <locale>
#include <regex>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "class.h"

enum TokenType {
    BUFFER = 100,
    BP_1 = 1,
    DBP_1 = 2,
    BP_2 = 3,
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
#define LEFT_BORDER -5
#define RIGHT_BORDER 5

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
                            vector<vector<double>> &coordinate_arr, vector<pair<double, double>> intervals);

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

bool checkFunctionNames(const std::string &expression, std::string &error);

// scope_of_definition.cpp

void print_intervals(vector<pair<double, double>> &intervals);
void check_nan_by_range(vector<vector<double>> &output_arr, unsigned int &i_out_arr, vector<bool> &nan_arr);
void create_intervals(vector<bool> &nan_arr, vector<pair<double, double>> &intervals);

// break_points.cpp

void search_break_points(vector<pair<double, double>> &intervals, vector<pair<double, int>> &break_points,
                         vector<vector<double>> &output_arr, unsigned int &i_out_arr);
void print_break_points(vector<pair<double, int>> &break_points);

// asymptote.cpp

void search_vertical_asymptote(vector<pair<double, double>> &intervals, vector<vector<double>> &output_arr,
                               unsigned int &i_out_arr, vector<double> &vertical_asymptote);
void print_vertical_asymptote(vector<double> &vertical_asymptote);
pair<double, double> search_horizontal_asymptote(vector<pair<double, double>> &intervals,
                                                 vector<vector<double>> &output_arr,
                                                 unsigned int &i_out_arr);

#endif  // PARSER_H