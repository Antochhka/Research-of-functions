#include "analysis.h"
#include "calculation.h"
#include "class.h"
#include "parser.h"
#include "validator.h"

using namespace std;
// Функция для проверки, является ли функция периодичной
bool isFunctionPeriodic(vector<vector<double>> &output_arr, unsigned int &i_out_arr) {
    double left_x = LEFT_BORDER;
    double right_x = RIGHT_BORDER;
    const double epsilon = 0.001;  // Погрешность для сравнения значений
    const double min_period_threshold = 2 * epsilon;  // Минимальный допустимый период
    double y_left = calculator(output_arr, i_out_arr, left_x);

    // Проверяем значения функции на интервале от LEFT_BORDER до RIGHT_BORDER
    for (double x = left_x + min_period_threshold; x <= right_x; x += epsilon) {
        double y_current = calculator(output_arr, i_out_arr, x);
        // Проверяем, совпадает ли значение с y на границе LEFT_BORDER
        if (fabs(y_current - y_left) < epsilon) {
            return true;  // Функция является периодичной
        }
    }
    return false;  // Функция не является периодичной
}

// Функция для нахождения периода функции, если она периодичная
std::string findFunctionPeriod(vector<vector<double>> &output_arr, unsigned int &i_out_arr) {
    double left_x = LEFT_BORDER;
    const double epsilon = 0.0001;  // Погрешность для сравнения значений
    const vector<double> periods = {M_PI / 2, M_PI, 3 * M_PI / 2,
                                    2 * M_PI};  // Возможные значения для периода
    double y_left = calculator(output_arr, i_out_arr, left_x);

    // Создаем временную переменную для хранения x, чтобы избежать проблем с
    // ссылкой
    for (double period : periods) {
        double current_x = left_x + period;
        double y_at_period = calculator(output_arr, i_out_arr, current_x);
        // Проверяем, совпадает ли значение с y на границе LEFT_BORDER с учетом
        // погрешности
        if (fabs(y_at_period - y_left) < epsilon) {
            return "Function is periodic with period: " + to_string(period);
        }
    }

    // Если совпадения не найдены, возвращаем информацию, что период не определен
    return "Function is not periodic on the given interval";
}

// 24.11.24 Добавлена читаемость и комментарии