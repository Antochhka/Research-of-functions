#include "analysis.h"
#include "calculation.h"
#include "parser.h"
#include "validator.h"

// Функция для вывода интервалов, где функция определена
void print_intervals(vector<pair<double, double>> &intervals) {
    cout << "### SCOPE OF DEFINITION ###" << endl;
    for (unsigned int i = 0; i < intervals.size(); i++) {
        // Вывод каждого интервала, где определена функция
        printf(" [%lf : %lf]\n", intervals[i].first, intervals[i].second);
    }
}

// Функция для проверки наличия NaN значений в диапазоне значений функции
// Заполняет массив nan_arr, где false - в случае NaN или бесконечности
void check_nan_by_range(vector<vector<double>> &output_arr, unsigned int &i_out_arr, vector<bool> &nan_arr,
                        string &input) {
    bool is_tg;
    (input.find("tan") == string::npos) ? is_tg = false : is_tg = true;

    for (unsigned int i = 0; i < nan_arr.size(); i++) {
        double x = LEFT_BORDER + i * SHIFT;  // Определяем текущее значение X
        double res = calculator(output_arr, i_out_arr,
                                x);  // Вычисляем значение функции в точке X

        // Проверка результата на NaN или бесконечность
        if (std::isnan(res) || std::isinf(res) || (is_tg && fabs(res) > 99)) {
            nan_arr[i] = false;  // Если NaN или бесконечность, значение недоступно
        } else {
            nan_arr[i] = true;  // Если значение определено, устанавливаем true
        }
    }
}

// Функция для создания интервалов, где функция определена, на основе массива
// nan_arr
void create_intervals(vector<bool> &nan_arr, vector<pair<double, double>> &intervals) {
    bool open = false;         // Флаг для отслеживания начала интервала
    double start = 0.0;        // Начало интервала
    double const INF = 1e+10;  // Приближенное значение для бесконечности
    double const EPSILON = 8.85 * 1e-12;  // Малое значение для сравнения, чтобы избежать ошибок округления

    // Определение начального интервала, если функция определена с левой границы
    if (nan_arr[0]) {
        start = -INF;
        open = true;
    }

    // Проход по массиву nan_arr для создания интервалов
    for (unsigned int i = 1; i < nan_arr.size(); i++) {
        if (open == true && !nan_arr[i]) {
            // Закрытие текущего интервала, если функция больше не определена
            open = false;
            double end = LEFT_BORDER + i * SHIFT;  // Определяем конец интервала
            intervals.push_back({start, end});     // Добавляем интервал в список
        } else if (open == false && nan_arr[i]) {
            // Открытие нового интервала, если функция стала определенной
            open = true;
            if (intervals.size() != 0) {
                start =
                    intervals[intervals.size() - 1].second;  // Начало нового интервала - конец предыдущего
            } else {
                start = LEFT_BORDER + i * SHIFT - SHIFT;  // Начало интервала с корректировкой на смещение
                if (fabs(start) <= EPSILON) {
                    start = 0.0;  // Если значение близко к нулю, устанавливаем его равным
                                  // нулю для точности
                }
            }
        }
    }

    // Добавление конечного интервала, если он открыт до конца массива
    if (open) {
        double end = INF;  // Устанавливаем конец интервала как бесконечность
        intervals.push_back({start, end});  // Добавляем интервал в список
    }
}

// 24.11.24 Добавлена читаемость и комментарии