#include "analysis.h"
#include "calculation.h"
#include "parser.h"
#include "validator.h"

// Функция для поиска вертикальной асимптоты на основе интервалов
void search_vertical_asymptote(vector<pair<double, double>> &intervals, vector<vector<double>> &output_arr,
                               unsigned int &i_out_arr, vector<double> &vertical_asymptote) {
    const double EPSILON = 8.85 * 1e-12;  // Малое значение для вычисления пределов слева и справа
    double INF = 1e+10;  // Приближенное значение для бесконечности

    // Проверка, если один из интервалов не уходит на бесконечность
    if (intervals[0].first != -INF || intervals[intervals.size() - 1].second != INF) {
        double point = 0.0;
        // Определяем точку для анализа, в зависимости от наличия интервала, не
        // уходящего в бесконечность
        if (intervals[0].first != -INF) {
            point = intervals[0].first;
        } else {
            point = intervals[intervals.size() - 1].second;
        }

        // Определение пределов слева и справа от точки
        double left_limit = point - EPSILON;
        double right_limit = point + EPSILON;
        left_limit = calculator(output_arr, i_out_arr,
                                left_limit);  // Вычисление значения функции слева
        right_limit = calculator(output_arr, i_out_arr,
                                 right_limit);  // Вычисление значения функции справа

        // Если хотя бы один из пределов не является NaN, значит это вертикальная
        // асимптота
        if (!std::isnan(left_limit) || !std::isnan(right_limit)) {
            vertical_asymptote.push_back(point);
        }
    }

    // Проверка на совпадение концов интервалов (непрерывность)
    for (unsigned int i = 1; i < intervals.size(); i++) {
        if (intervals[i - 1].second == intervals[i].first) {
            vertical_asymptote.push_back(intervals[i].first);
        }
    }
}

// Функция для поиска горизонтальной асимптоты на основе интервалов
pair<double, double> search_horizontal_asymptote(vector<pair<double, double>> &intervals,
                                                 vector<vector<double>> &output_arr,
                                                 unsigned int &i_out_arr, std::pair &k_b) {
    double INF = 1e+10;  // Приближенное значение для бесконечности

    // Проверка, если интервалы включают бесконечность на границах
    if (intervals[0].first == -INF && intervals[intervals.size() - 1].second == INF) {
        double k = calculator(output_arr, i_out_arr, INF) / INF;  // Определение наклона асимптоты

        INF = 1e+5;  // Изменение значения INF для точности расчёта
        double b = (calculator(output_arr, i_out_arr, INF)) - (k * INF);  // Вычисление свободного члена b

        // Корректировка значения b до целого значения
        if (b < 0) {
            b = std::floor(b);
        } else {
            b = std::ceil(b);
        }

        printf("k = %lf b = %lf\n", k,
               b);  // Вывод наклона и свободного члена асимптоты
        return {k, b};
    }

    // Возвращаем значение по умолчанию, если условие не выполнено
    return {0.0, 0.0};  // Можно изменить на любое значение, подходящее вашему случаю
}

// Функция для вывода вертикальных асимптот
void print_vertical_asymptote(vector<double> &vertical_asymptote) {
    cout << "### VERTICAL ASYMPTOTE ###" << endl;
    for (unsigned int i = 0; i < vertical_asymptote.size(); i++) {
        cout << "Vertical asymptote at x = " << vertical_asymptote[i]
             << endl;  // Вывод координаты вертикальной асимптоты
    }
}

// 24.11.24 Добавлена читаемость и комментарии