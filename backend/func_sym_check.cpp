#include <cmath>
#include <iostream>
#include <vector>

#include "analysis.h"
#include "calculation.h"
#include "class.h"
#include "parser.h"
#include "validator.h"

using namespace std;

// Функция для проверки четности функции
std::string checkFunctionSymmetry(vector<vector<double>> &output_arr, unsigned int &i_out_arr) {
    double left_x = LEFT_BORDER;
    double right_x = RIGHT_BORDER;

    // Вычисляем значения функции при крайних значениях x
    double y_left = calculator(output_arr, i_out_arr, left_x);
    double y_right = calculator(output_arr, i_out_arr, right_x);

    // Проверяем первое условие - если значения равны, функция четная
    if (fabs(y_left - y_right) < 1e-9) {  // Сравнение с учетом малой погрешности
        return "Функция четная";
    }

    // Проверяем второе условие - если значения противоположны, функция нечетная
    if (fabs(y_left + y_right) < 1e-9) {  // Сравнение с учетом малой погрешности
        return "Функция нечетная";
    }

    // Если ни одно из условий не выполняется, функция не является ни четной, ни
    // нечетной
    return "Функция не является ни четной, ни нечетной.";
}

// 24.11.24 Добавлена читаемость и комментарии