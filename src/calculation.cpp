#include <math.h>

#include "class.h"
#include "parser.h"

double calculator(vector<vector<double>> &output_arr, unsigned int &i_out_arr, double &coordinate_x) {
    Stack<double> expression;
    for (unsigned int i = 0; i < i_out_arr; i++) {
        unsigned int codif = output_arr[i][CODIF];
        double num1 = 0.0;
        double num2 = 0.0;
        switch (codif) {
            case NUM:
                expression.push(output_arr[i][DATA]);
                break;
            case X:
                expression.push(coordinate_x);
                break;
            case PLUS:
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 + num1);
                break;
            case MINUS:
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 - num1);
                break;
            case MULT:
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 * num1);
                break;
            case DIV:
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(num2 / num1);
                break;
            case SIN:
                num1 = expression.pop();
                expression.push(sin(num1));
                break;
            case COS:
                num1 = expression.pop();
                expression.push(cos(num1));
                break;
            case TG:
                num1 = expression.pop();
                expression.push(tan(num1));
                break;
            case ASIN:
                num1 = expression.pop();
                expression.push(asin(num1));
                break;
            case ACOS:
                num1 = expression.pop();
                expression.push(acos(num1));
                break;
            case ATG:
                num1 = expression.pop();
                expression.push(atan(num1));
                break;
            case SQRT:
                num1 = expression.pop();
                expression.push(sqrt(num1));
                break;
            case EXP:
                num1 = expression.pop();
                expression.push(exp(num1));
                break;
            case LOG:
                num1 = expression.pop();
                expression.push(log2(num1));
                break;
            case LOG10:
                num1 = expression.pop();
                expression.push(log10(num1));
                break;
            case POW:
                num1 = expression.pop();
                num2 = expression.pop();
                expression.push(pow(num2, num1));
                break;
            case UMINUS:
                num1 = expression.pop();
                expression.push(-num1);
                break;
            default:
                break;
        }
    }
    return expression.pop();
}

void calculating_coordinate(vector<vector<double>> &output_arr, unsigned int &i_out_arr,
                            vector<vector<double>> &coordinate_arr, vector<pair<double, double>> intervals) {
    const double EPSILON = 8.85 * 1e-12;  // Маленькое число для сравнения
    const double INF = 1e+10;
    for (unsigned int i = 0; i < intervals.size(); i++) {
        double x = intervals[i].first + SHIFT;
        while (x < intervals[i].second - EPSILON && x <= RIGHT_BORDER) {
            if (x == -INF + SHIFT) {
                x = LEFT_BORDER;
            }
            double y = calculator(output_arr, i_out_arr, x);
            vector<double> new_coordinates = {y, x};
            coordinate_arr.push_back(new_coordinates);
            x += SHIFT;
        }
    }
}

void print_coordinate_arr(vector<vector<double>> &coordinate_arr) {
    for (unsigned int i = 0; i < coordinate_arr.size(); i++) {
        printf(" [X]: %lf, [Y]: %lf\n", coordinate_arr[i][X], coordinate_arr[i][Y]);
    }
}