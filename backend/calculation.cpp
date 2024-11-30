#include "calculation.h"

#include <math.h>

#include "class.h"
#include "parser.h"

// Функция для выполнения вычислений на основе выходного массива

// Используется стек для работы с постфиксной записью (обратная польская запись)
double calculator(vector<vector<double>> &output_arr, unsigned int &i_out_arr, double &coordinate_x) {
    Stack<double> expression;  // Стек для хранения значений во время вычислений
    for (unsigned int i = 0; i < i_out_arr; i++) {
        unsigned int codif = output_arr[i][CODIF];  // Получение кодировки токена
        double num1 = 0.0;  // Переменная для первого операнда
        double num2 = 0.0;  // Переменная для второго операнда

        // Выполнение операций в зависимости от типа токена
        switch (codif) {
            case NUM:  // Число
                expression.push(output_arr[i][DATA]);
                break;
            case X:  // Переменная X
                expression.push(coordinate_x);
                break;
            case PLUS:  // Сложение
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 + num1);
                break;
            case MINUS:  // Вычитание
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 - num1);
                break;
            case MULT:  // Умножение
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 * num1);
                break;
            case DIV:  // Деление
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 / num1);
                break;
            case SIN:  // Синус
                num1 = expression.pop();
                expression.push(sin(num1));
                break;
            case COS:  // Косинус
                num1 = expression.pop();
                expression.push(cos(num1));
                break;
            case TG:  // Тангенс
                num1 = expression.pop();
                expression.push(tan(num1));
                break;
            case ASIN:  // Арксинус
                num1 = expression.pop();
                expression.push(asin(num1));
                break;
            case ACOS:  // Арккосинус
                num1 = expression.pop();
                expression.push(acos(num1));
                break;
            case ATG:  // Арктангенс
                num1 = expression.pop();
                expression.push(atan(num1));
                break;
            case SQRT:  // Квадратный корень
                num1 = expression.pop();
                expression.push(sqrt(num1));
                break;
            case EXP:  // Экспонента
                num1 = expression.pop();
                expression.push(exp(num1));
                break;
            case LOG:  // Логарифм по основанию 2
                num1 = expression.pop();
                expression.push(log2(num1));
                break;
            case LOG10:  // Десятичный логарифм
                num1 = expression.pop();
                expression.push(log10(num1));
                break;
            case POW:  // Возведение в степень
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(pow(num2, num1));
                break;
            case UMINUS:  // Унарный минус
                num1 = expression.pop();
                expression.push(-num1);
                break;
            default:
                break;
        }
    }
    return expression.pop();  // Возвращаем итоговое значение из стека
}

// Функция для расчета координат на основе интервалов
void calculating_coordinate(vector<vector<double>> &output_arr, unsigned int &i_out_arr,
                            vector<vector<double>> &coordinate_arr, vector<pair<double, double>> intervals) {
    const double EPSILON = 8.85 * 1e-12;  // Маленькое число для сравнения, чтобы избежать погрешностей
    const double INF = 1e+10;  // Приближенное значение для бесконечности

    // Проход по каждому интервалу для расчета значений функции
    for (unsigned int i = 0; i < intervals.size(); i++) {
        double x = intervals[i].first + SHIFT;  // Начальная точка интервала с добавлением небольшого смещения
        while (x < intervals[i].second - EPSILON && x <= RIGHT_BORDER) {
            if (x == -INF + SHIFT) {  // Проверка для корректного начала с левой границы
                x = LEFT_BORDER;
            }
            double y = calculator(output_arr, i_out_arr,
                                  x);  // Вычисление значения функции в точке x
            vector<double> new_coordinates = {y, x};  // Создание нового вектора координат
            coordinate_arr.push_back(new_coordinates);  // Добавление координат в массив
            x += SHIFT;  // Переход к следующему значению с шагом SHIFT
        }
    }
}

// Функция для вывода массива координат на экран
void print_coordinate_arr(vector<vector<double>> &coordinate_arr) {
    for (unsigned int i = 0; i < coordinate_arr.size(); i++) {
        // Вывод координат X и Y
        printf(" [X]: %lf, [Y]: %lf\n", coordinate_arr[i][X], coordinate_arr[i][Y]);
    }
}

// 24.11.24 Добавлена читаемость и комментарии