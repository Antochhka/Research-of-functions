#include "derivatives.h"

#include <tuple>

#include "parser.h"

#define POINT 1           //Индекс критической точки в массиве
#define LEFT_INTERVAL 0   //Значение левого интервала
#define RIGHT_INTERVAL 2  //Значение правого интервала
#define MAX_OR_MIN 3      //Точка является максимумом или минимумом
#define MIN_POINT -1      //равно -1, если точка является точкой минимума
#define MAX_POINT 1       //равно 1, если точка является точкой максимума
#define NO_POINT -2  //равно -2, если точка не является точкой минимума или максимума
#define NEGATIVE_ZONE -1  //равно -1, Зона, где график убывает
#define POSITIVE_ZONE 1   //равно 1, Зона, где график возрастает
#define CONVEX_ZONE 1     //равно 1, Зона, где график выпуклый вниз
#define CONCAVE_ZONE -1   //равно -1, Зона, где график выпуклый вверх

// Поиск интервалов монотонности
void search_monotony_intervals(vector<pair<double, double>> &intervals,
                               vector<vector<double>> &critical_points,
                               vector<std::tuple<double, double, int>> &monotony_intervals,
                               RCP<const Basic> &first_derivative_expression) {
    // Символьная переменная `x`
    RCP<const Symbol> x = symbol("x");

    // Для проверки знака производной
    map_basic_basic variable_substitution;

    const double EPSILON = 8.85 * 1e-30;

    //Проверяем, зависит ли 2

    for (const auto &interval : intervals) {
        double left_border = interval.first;
        double right_border = interval.second;

        // Сначала извлекаем критические точки внутри текущего интервала
        vector<double> inside_critical_points;
        for (const auto &point : critical_points) {
            double critical_x = point[POINT];
            if (critical_x > left_border && critical_x < right_border) {
                inside_critical_points.push_back(critical_x);
            }
        }

        // Добавляем границы интервала для проверки
        inside_critical_points.push_back(left_border);
        inside_critical_points.push_back(right_border);

        // Сортируем точки
        sort(inside_critical_points.begin(), inside_critical_points.end());

        // Определяем монотонность на каждом подинтервале
        for (size_t i = 1; i < inside_critical_points.size(); ++i) {
            double begin_interval = inside_critical_points[i - 1];
            double end_interval = inside_critical_points[i];
            double midpoint = (begin_interval + end_interval) / 2;

            // Проверяем знак первой производной в середине интервала
            variable_substitution[x] = real_double(midpoint);
            RCP<const Basic> substituted_derivative =
                first_derivative_expression->subs(variable_substitution);  // Подстановка

            double derivative_value = eval_double(*substituted_derivative);
            int monotony_sign = (derivative_value > EPSILON) ? 1 : (derivative_value < -EPSILON) ? -1 : 0;

            // Если знак неопределён, игнорируем интервал
            if (monotony_sign != 0) {
                monotony_intervals.push_back({begin_interval, end_interval, monotony_sign});
            }
        }
    }

    // Отладочная информация
    cout << "monotony of intervals:\n";
    for (size_t i = 0; i < monotony_intervals.size(); ++i) {
        auto [start, end, monotony] = monotony_intervals[i];
        cout << "interval [" << start << ", " << end << "]: " << (monotony == 1 ? "UP" : "DOWN") << endl;
    }
}

void search_convexity_concavity_intervals(
    vector<pair<double, double>> &intervals, vector<vector<double>> &second_critical_points,
    vector<std::tuple<double, double, int>> &convexity_concavity_intervals,
    RCP<const Basic> &second_derivative_expression) {
    const double EPSILON = 8.85 * 1e-30;
    RCP<const Symbol> x = symbol("x");
    map_basic_basic variable_substitution;

    string derivative_in_string = str(*second_derivative_expression);

    for (const auto &intreval : intervals) {
        double left_border = intreval.first;
        double right_border = intreval.second;

        // Сначала извлекаем критические точки внутри текущего интервала
        vector<double> inside_critical_points;
        for (size_t i = 0; i < second_critical_points.size(); ++i) {
            double critical_x = second_critical_points[i][POINT];
            if (critical_x > left_border && critical_x < right_border) {
                inside_critical_points.push_back(critical_x);
            }
        }

        inside_critical_points.push_back(left_border);
        inside_critical_points.push_back(right_border);

        // Сортируем точки
        sort(inside_critical_points.begin(), inside_critical_points.end());

        // Идем по новым интервалам и чекаем 2 производную
        for (size_t i = 0; i < inside_critical_points.size() - 1; ++i) {
            double left_point = inside_critical_points[i];
            double right_point = inside_critical_points[i + 1];
            double midlle_point = (left_point + right_point) / 2;
            variable_substitution[x] = real_double(midlle_point);
            double res = eval_double(*second_derivative_expression->subs(variable_substitution));
            if (res > EPSILON) {
                convexity_concavity_intervals.push_back({left_point, right_point, CONVEX_ZONE});
            } else {
                convexity_concavity_intervals.push_back({left_point, right_point, CONCAVE_ZONE});
            }
        }
    }

    //Отладочная информация
    cout << "convexity of intervals:\n";
    for (size_t i = 0; i < convexity_concavity_intervals.size(); ++i) {
        auto [start, end, zone] = convexity_concavity_intervals[i];
        cout << "interval [" << start << ", " << end << "]: " << (zone == CONVEX_ZONE ? "CONVEX" : "CONCAVE")
             << endl;
    }
}

void search_critical_points(vector<vector<double>> &first_critical_points,
                            vector<vector<double>> &second_critical_points,
                            RCP<const Basic> &first_derivative_expr, RCP<const Basic> &second_derivative_expr,
                            string input) {
    // Парсим выражение
    RCP<const Basic> parsed_expr = parse(input);

    // Символьная переменная x
    RCP<const Symbol> x = symbol("x");

    // Находим первую производную
    first_derivative_expr = diff(parsed_expr, x);

    // Находим вторую производную
    second_derivative_expr = diff(first_derivative_expr, x);

    cout << "my_func: " << *parsed_expr << endl;
    cout << "first_derivative: " << *first_derivative_expr << endl;
    cout << "second_derivative: " << *second_derivative_expr << endl;

    // Объект для замены переменных
    map_basic_basic variable_substitution;
    double x_value = LEFT_BORDER;
    const double EPSILON = 8.85 * 1e-12;

    // Поиск критических точек первой производной
    while (x_value < RIGHT_BORDER - EPSILON) {
        variable_substitution[x] = real_double(x_value);
        RCP<const Basic> substituted_derivative = first_derivative_expr->subs(variable_substitution);
        double res = eval_double(*substituted_derivative);
        res = round(res * 100) / 100;
        if (fabs(res) < EPSILON) {
            variable_substitution[x] = real_double(x_value - SHIFT);
            double left_zone = eval_double(*first_derivative_expr->subs(variable_substitution));
            variable_substitution[x] = real_double(x_value + SHIFT);
            double right_zone = eval_double(*first_derivative_expr->subs(variable_substitution));
            double temp_x;
            fabs(x_value) < EPSILON ? temp_x = 0.0 : temp_x = x_value;

            if (left_zone < EPSILON && right_zone > EPSILON) {
                first_critical_points.push_back({NEGATIVE_ZONE, temp_x, POSITIVE_ZONE, MIN_POINT});
            } else if (left_zone > EPSILON && right_zone < EPSILON) {
                first_critical_points.push_back({POSITIVE_ZONE, temp_x, NEGATIVE_ZONE, MAX_POINT});
            } else if (left_zone < EPSILON && right_zone < EPSILON) {
                first_critical_points.push_back({NEGATIVE_ZONE, temp_x, NEGATIVE_ZONE, NO_POINT});
            } else if (left_zone > EPSILON && right_zone > EPSILON) {
                first_critical_points.push_back({POSITIVE_ZONE, temp_x, POSITIVE_ZONE, NO_POINT});
            }
        }
        x_value += SHIFT;
    }

    // Вывод критических точек первой производной
    for (unsigned int i = 0; i < first_critical_points.size(); i++) {
        cout << "Point " << i + 1 << ": " << first_critical_points[i][POINT];
        if (first_critical_points[i][MAX_OR_MIN] == MIN_POINT) {
            cout << " is a local minimum";
        } else if (first_critical_points[i][MAX_OR_MIN] == MAX_POINT) {
            cout << " is a local maximum";
        } else if (first_critical_points[i][MAX_OR_MIN] == NO_POINT) {
            cout << " is neither a local minimum nor a local maximum";
        }
        cout << endl;
    }

    // Сбрасываем x_value для второй производной
    x_value = LEFT_BORDER;

    // Поиск критических точек второй производной
    while (x_value < RIGHT_BORDER - EPSILON) {
        variable_substitution[x] = real_double(x_value);
        RCP<const Basic> substituted_second_derivative = second_derivative_expr->subs(variable_substitution);
        double res = eval_double(*substituted_second_derivative);
        res = round(res * 100) / 100;
        // cout << "x = " << x_value << ", f''(x) = " << res << endl;
        if (fabs(res) < EPSILON) {
            variable_substitution[x] = real_double(x_value - SHIFT);
            double left_zone = eval_double(*second_derivative_expr->subs(variable_substitution));
            variable_substitution[x] = real_double(x_value + SHIFT);
            double right_zone = eval_double(*second_derivative_expr->subs(variable_substitution));
            double temp_x;
            fabs(x_value) < EPSILON ? temp_x = 0.0 : temp_x = x_value;

            if (left_zone < 0 && right_zone > 0) {
                second_critical_points.push_back(
                    {NEGATIVE_ZONE, temp_x, POSITIVE_ZONE, MIN_POINT});  // Локальный минимум 2 -й производной
            } else if (left_zone > 0 && right_zone < 0) {
                second_critical_points.push_back({POSITIVE_ZONE, temp_x, NEGATIVE_ZONE,
                                                  MAX_POINT});  // Локальный максимум 2 -й производной
            }
        }
        x_value += SHIFT;
    }

    // Вывод критических точек второй производной
    for (unsigned int i = 0; i < second_critical_points.size(); i++) {
        cout << "Second Derivative Point " << i + 1 << ": " << second_critical_points[i][POINT]
             << (second_critical_points[i][MAX_OR_MIN] == -1 ? " - local concave up (minimum curvature)"
                                                             : " - local concave down (maximum curvature)")
             << endl;
    }
}