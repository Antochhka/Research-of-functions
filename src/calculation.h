#ifndef CALCULATION_H
#define CALCULATION_H

#include "class.h"  // Подключение заголовочного файла, содержащего класс Stack

// Функция для выполнения вычислений на основе выходного массива
double calculator(vector<vector<double>> &output_arr, unsigned int &i_out_arr, double &coordinate_x);

// Функция для расчета координат на основе интервалов
void calculating_coordinate(vector<vector<double>> &output_arr, unsigned int &i_out_arr,
                            vector<vector<double>> &coordinate_arr, vector<pair<double, double>> intervals);

// Функция для печати массива координат
void print_coordinate_arr(vector<vector<double>> &coordinate_arr);

#endif  // CALCULATION_H

// 24.11.24 Добавлена читаемость и комментарии