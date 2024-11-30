#include "analysis.h"
#include "calculation.h"
#include "derivatives.h"
#include "parser.h"
#include "validator.h"

// Функция для поиска точек разрыва на основе интервалов
void search_break_points(vector<pair<double, double>> &intervals, vector<pair<double, int>> &break_points,
                         vector<vector<double>> &output_arr, unsigned int &i_out_arr, string input,
                         string break_point_for_tan) {
    if (input.find("tan") != string::npos) {
        // Парсим строку в объект SymEngine
        RCP<const Basic> expr = parse(input);

        RCP<const Symbol> x = symbol("x");

        RCP<const FunctionSymbol> func = rcp_static_cast<const FunctionSymbol>(expr);

        vec_basic args = func->get_args();

        RCP<const Basic> lhs = args[0];  //аргумент внутри тангенса

        RCP<const Basic> rhs = div(pi, integer(2));  // Это "pi/2"

        auto sub_res = sub(lhs, rhs);

        auto res = solve(sub_res, x);

        break_point_for_tan = str(*res);

        cout << "break_point_for_tan = " << break_point_for_tan << endl;

    }

    double EPSILON = 8.85 * 1e-12;  // Малое значение для вычисления пределов
                                    // слева и справа от точки разрыва
    double INF = 1e+10;  // Приближенное значение для бесконечности

    // Цикл для поиска точек разрыва между соседними интервалами
    for (unsigned int i = 0; i < intervals.size() - 1; i++) {
        // Проверка, если граница одного интервала совпадает с началом следующего
        if (intervals[i].second == intervals[i + 1].first) {
            double break_point = intervals[i].second;
            double left_limit = break_point - EPSILON;  // Определение предела слева от точки разрыва
            double right_limit = break_point + EPSILON;  // Определение предела справа от точки разрыва
            left_limit = calculator(output_arr, i_out_arr,
                                    left_limit);  // Вычисление значения функции слева
            right_limit = calculator(output_arr, i_out_arr,
                                     right_limit);  // Вычисление значения функции справа

            // Определение типа точки разрыва
            if (left_limit == right_limit && (fabs(left_limit) < INF && fabs(right_limit) < INF)) {
                // Устранимая точка разрыва 1-го рода
                break_points.push_back({intervals[i].second, DBP_1});
            } else if (fabs(left_limit) < INF && fabs(right_limit) < INF) {
                // Точка разрыва 1-го рода
                break_points.push_back({intervals[i].second, BP_1});
            } else if (fabs(left_limit) >= INF && fabs(right_limit) >= INF) {
                // Точка разрыва 2-го рода
                break_points.push_back({intervals[i].second, BP_2});
            }
        }
    }
}

// Функция для вывода найденных точек разрыва
void print_break_points(vector<pair<double, int>> &break_points) {
    cout << "### BREAK POINT ###" << endl;
    for (unsigned int i = 0; i < break_points.size(); i++) {
        // В зависимости от типа точки разрыва, выводим соответствующее сообщение
        if (break_points[i].second == DBP_1) {
            printf("[%lf] - устранимая точка разрыва 1 - го рода", break_points[i].first);
        } else if (break_points[i].second == BP_1) {
            printf("[%lf] - точка разрыва 1 - го рода", break_points[i].first);
        } else {
            printf("[%lf] - точка разрыва 2 - го рода", break_points[i].first);
        }
        cout << endl;  // Переход на новую строку для каждого вывода
    }
}

// 24.11.24 Добавлена читаемость и комментарии