#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "class.h"  // Подключение заголовочного файла, содержащего класс Stack

// ========================= scope_of_definition.cpp =========================

// Функция для печати интервалов
void print_intervals(vector<pair<double, double>> &intervals);

// Функция для проверки наличия NaN значений в диапазоне
void check_nan_by_range(vector<vector<double>> &output_arr, unsigned int &i_out_arr, vector<bool> &nan_arr,
                        string &input);

// Функция для создания интервалов на основе массива NaN значений
void create_intervals(vector<bool> &nan_arr, vector<pair<double, double>> &intervals);

// ========================= break_points.cpp =========================

// Функция для поиска точек разрыва
void search_break_points(vector<pair<double, double>> &intervals, vector<pair<double, int>> &break_points,
                         vector<vector<double>> &output_arr, unsigned int &i_out_arr);

// Функция для печати точек разрыва
void print_break_points(vector<pair<double, int>> &break_points);

// ========================= asymptote.cpp =========================

// Функция для поиска вертикальной асимптоты
void search_vertical_asymptote(vector<pair<double, double>> &intervals, vector<vector<double>> &output_arr,
                               unsigned int &i_out_arr, vector<double> &vertical_asymptote);

// Функция для печати вертикальной асимптоты
void print_vertical_asymptote(vector<double> &vertical_asymptote);

// Функция для поиска горизонтальной асимптоты
void search_horizontal_asymptote(vector<pair<double, double>> &intervals, vector<vector<double>> &output_arr,
                                 unsigned int &i_out_arr, vector<double> &k_b_index);

// Функция проверки вхождения тангенса
bool tg_check(const vector<vector<double>> &output_arr);

// ========================= symmetry_and_periodicity.cpp
// =========================

// Функция для проверки симметрии функции
std::string checkFunctionSymmetry(vector<vector<double>> &output_arr, unsigned int &i_out_arr);

// Функция для проверки периодичности функции
bool isFunctionPeriodic(vector<vector<double>> &output_arr, unsigned int &i_out_arr);

// Функция для нахождения периода функции
std::string findFunctionPeriod(vector<vector<double>> &output_arr, unsigned int &i_out_arr);

#endif  // ANALYSIS_H

// 24.11.24 Добавлена читаемость и комментарии