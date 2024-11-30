#include "analysis.h"
#include "calculation.h"
#include "derivatives.h"
#include "parser.h"
#include "validator.h"

int main() {
    setlocale(LC_ALL, "Ru");  // Устанавливаем локализацию для поддержки русского языка
    string input;
    getline(cin, input);

    // Проверяем корректность введенного математического выражения
    if (validateMathExpression(input)) {
        vector<vector<double>> output_arr;  // Массив для хранения выходных данных (обратная польская запись)
        vector<bool> nan_arr;  // Массив для хранения информации о наличии NaN значений
        vector<pair<double, double>> intervals;  // Вектор для хранения интервалов, где функция определена
        vector<pair<double, int>> break_points;  // Вектор для хранения точек разрыва функции
        vector<vector<double>> coordinate_arr;  // Вектор для хранения координатных точек (X, Y)
        vector<double> vertical_asymptote;  // Вектор для хранения вертикальных асимптот
        pair<double, double> horizontal_asymptote;  // Пара горизонтальной асимптоты
        unsigned int i_out_arr = 0;  // Индекс для отслеживания выхода из массива
        vector<vector<double>> first_critical_points;  //Критические точки первой производной
        vector<vector<double>> second_critical_points;  //Критические точки второй производной
        RCP<const Basic> first_derivative_expression;  //Выражение первой производной
        RCP<const Basic> second_derivative_expression;  //Выражение второй производной
        vector<std::tuple<double, double, int>> monotony_intervals;  //Интервалы монотонности
        vector<std::tuple<double, double, int>>
            convexity_concavity_intervals;  // Интервалы выпуклости/вогнутости
        string break_point_for_tan;  // Точка разрыва, если подаваемая функция tan

        // Инициализация выходного массива
        output_arr.resize(BUFFER);
        for (int i = 0; i < BUFFER; i++) {
            output_arr[i].resize(2);  // Каждый элемент массива хранит кодировку и данные
        }

        // Парсим входное выражение и записываем его в выходной массив
        parser(output_arr, i_out_arr, input);

        int size = RIGHT_BORDER * 2 * 1 / SHIFT + 1;  // Определяем размер массива для NaN значений
        nan_arr.resize(size);  // Устанавливаем размер массива для NaN значений

        // Проверка наличия NaN значений в диапазоне и создание интервалов, где функция определена
        check_nan_by_range(output_arr, i_out_arr, nan_arr, input);
        create_intervals(nan_arr, intervals);

        // Расчет координатных точек на основе интервалов, где функция определена
        calculating_coordinate(output_arr, i_out_arr, coordinate_arr, intervals);

        // Печать рассчитанных координат
        print_coordinate_arr(coordinate_arr);

        // Поиск точек разрыва функции
        search_break_points(intervals, break_points, output_arr, i_out_arr, input, break_point_for_tan);

        // Поиск вертикальных асимптот функции
        search_vertical_asymptote(intervals, output_arr, i_out_arr, vertical_asymptote);

        // Печать интервалов, точек разрыва и вертикальных асимптот
        print_intervals(intervals);
        print_break_points(break_points);
        print_vertical_asymptote(vertical_asymptote);

        // Поиск горизонтальной асимптоты и её вывод
        if (!vertical_asymptote.empty()) {
            horizontal_asymptote = search_horizontal_asymptote(intervals, output_arr, i_out_arr);
        }

        // Проверка симметрии функции и вывод результата
        std::string result = checkFunctionSymmetry(output_arr, i_out_arr);
        cout << result << endl;

        // Проверка периодичности функции и вывод результата
        bool is_periodic = isFunctionPeriodic(output_arr, i_out_arr);
        if (is_periodic) {
            std::string result = findFunctionPeriod(output_arr, i_out_arr);  // Поиск периода функции
            cout << result << endl;
        } else {
            cout << "Function is not periodic on the given interval" << endl;
        }

        // Поиск критических точек первой и второй производных
        search_critical_points(first_critical_points, second_critical_points, first_derivative_expression,
                               second_derivative_expression, input);

        //Области возрастания и убывания
        search_monotony_intervals(intervals, first_critical_points, monotony_intervals,
                                  first_derivative_expression);

        // Области выпуклости и вогнутости
        search_convexity_concavity_intervals(intervals, second_critical_points, convexity_concavity_intervals,
                                             second_derivative_expression);
    }
    return 0;
}

// 24.11.24 Добавлена читаемость и комментарии
