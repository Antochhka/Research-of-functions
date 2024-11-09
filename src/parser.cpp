// parser.cpp
#include "parser.h"

void parser(vector<vector<double>> &output_arr, unsigned int &i_out_arr, string &input) {
    unsigned int i = 0;
    double sum = 0.0;
    Stack<char> operations_stack;
    string operators = "+-*/^";
    string functions = "sctahgqelo";  // sin, cos, tan, asin, acos, atan, log, exp, sqrt,
    
    // Основной цикл для обработки входной строки
    while (input[i]) {
        char el = input[i];
        if (isdigit(el)) {  // Если символ - число, вызываем обработку числа
            i += is_digit(i, input, sum, output_arr, i_out_arr);
        } else if (el == 'x') {  // Если символ - переменная 'x'
            output_arr[i_out_arr][CODIF] = X;
            output_arr[i_out_arr][DATA] = NONE;
            i_out_arr++;
        } else if (operators.find(el) != string::npos || functions.find(el) != string::npos) {  // Если символ - оператор или функция
            // Проверка, является ли текущий минус унарным
            if (el == '-' && input[i - 1] != 'x' &&
                (i == 0 || input[i - 1] == '(' || input[i - 1] == ')' || (!isdigit(input[i - 1])))) {
                operations_stack.push('`');  // Добавляем унарный минус
            } else {
                char res = ' ';
                if (functions.find(el) != string::npos) {  // Если это функция (например, sin, cos)
                    i += is_func(i, input, res);
                    cout << res;  // Выводим символ функции
                } else {
                    res = el;  // Если это оператор
                }

                // Переносим операции с более высоким приоритетом из стека в выходной массив
                while (!operations_stack.is_empty() && operations_stack.peak() != '(' &&
                       (priority(res) < priority(operations_stack.peak()))) {
                    output_arr[i_out_arr][CODIF] = codifications(operations_stack.pop());
                    output_arr[i_out_arr][DATA] = NONE;
                    i_out_arr++;
                }

                operations_stack.push(res);  // Добавляем текущий оператор/функцию в стек
            }
        } else if (el == '(') {  // Если символ - открывающая скобка
            operations_stack.push(el);
        } else if (el == ')') {  // Если символ - закрывающая скобка
            while (operations_stack.peak() != '(') {  // Извлекаем все до открывающей скобки
                output_arr[i_out_arr][CODIF] = codifications(operations_stack.pop());
                output_arr[i_out_arr][DATA] = NONE;
                i_out_arr++;
            }
            operations_stack.pop();  // Удаляем открывающую скобку из стека
        }
        operations_stack.print_stack();  // Печать текущего содержимого стека (для отладки)
        print_arr(output_arr, i_out_arr);  // Печать выходного массива (для отладки)
        i++;
    }
    stack_in_output_arr(operations_stack, output_arr, i_out_arr);  // Переносим оставшиеся операции из стека в выходной массив
    std::cout << endl << endl << "FINISH:" << endl;
    print_arr(output_arr, i_out_arr);  // Печать окончательного выходного массива
}

void print_arr(vector<vector<double>> &output_arr, unsigned int &i_out_arr) {
    // Печать содержимого выходного массива
    std::cout << "In output string:" << endl;
    for (unsigned int i = 0; i < i_out_arr; i++) {
        printf("[%d]: codif: %.0lf elem: %.2lf\n", i, output_arr[i][0], output_arr[i][1]);
    }
    std::cout << endl;
}

unsigned int is_digit(unsigned int &i, string &input, double &sum, vector<vector<double>> &output_arr,
                      unsigned int &i_out_arr) {
    // Обработка числовых значений в строке и их добавление в выходной массив
    unsigned int indx = i;
    string double_num = "";
    while (indx < input.size() && (isdigit(input[indx]) || input[indx] == '.')) {
        double_num += input[indx];
        indx++;
    }
    sum = stod(double_num);
    output_arr[i_out_arr][CODIF] = NUM;
    output_arr[i_out_arr][DATA] = sum;
    i_out_arr++;
    sum = 0.0;
    return indx - i - 1;
}

unsigned int priority(char el) {
    // Определение приоритета операторов
    unsigned int priority = 0;
    switch (el) {
        case '(':
            priority = 0;
            break;
        case '+':
        case '-':
            priority = 1;
            break;
        case '*':
        case '/':
            priority = 2;
            break;
        case '^':
            priority = 3;  // Более высокий приоритет для возведения в степень
            break;
        default:
            priority = 3;
            break;
    }
    return priority;
}

unsigned int codifications(char el) {
    // Определение кодировки для операторов и функций
    unsigned int codification = 0;
    switch (el) {
        case '+':
            codification = PLUS;
            break;
        case '`':
            codification = UMINUS;
            break;
        case '-':
            codification = MINUS;
            break;
        case '*':
            codification = MULT;
            break;
        case '/':
            codification = DIV;
            break;
        case 's':
            codification = SIN;
            break;
        case 'c':
            codification = COS;
            break;
        case 't':
            codification = TG;
            break;
        case 'a':
            codification = ASIN;
            break;
        case 'h':
            codification = ACOS;
            break;
        case 'g':
            codification = ATG;
            break;
        case 'q':
            codification = SQRT;
            break;
        case 'e':
            codification = EXP;
            break;
        case 'l':
            codification = LOG;
            break;
        case 'o':
            codification = LOG10;
            break;
        case '^':
            codification = POW;
            break;
        default:
            codification = NONE;
            break;
    }
    return codification;
}

void stack_in_output_arr(Stack<char> &operations_stack, vector<vector<double>> &output_arr,
                         unsigned int &i_out_arr) {
    // Перенос всех оставшихся операторов из стека в выходной массив
    while (!operations_stack.is_empty()) {
        char el = operations_stack.pop();
        output_arr[i_out_arr][CODIF] = codifications(el);
        output_arr[i_out_arr][DATA] = NONE;
        i_out_arr++;
    }
}

unsigned int is_func(unsigned int &i, string &input, char &result) {
    // Проверка на наличие функции в текущей позиции строки
    unsigned int count = 0;
if (i + 2 < input.size() && input[i] == 's' && input[i + 1] == 'i' && input[i + 2] == 'n') {
        result = 's';
        count = 2;
    } else if (i + 2 < input.size() && input[i] == 'c' && input[i + 1] == 'o' && input[i + 2] == 's') {
        result = 'c';
        count = 2;
    } else if (i + 1 < input.size() && input[i] == 't' && input[i + 1] == 'g') {
        result = 't';
        count = 1;
    } else if (i + 3 < input.size() && input[i] == 'a' && input[i + 1] == 's' && input[i + 2] == 'i' && input[i + 3] == 'n') {
        result = 'a';
        count = 3;
    } else if (i + 3 < input.size() && input[i] == 'a' && input[i + 1] == 'c' && input[i + 2] == 'o' && input[i + 3] == 's') {
        result = 'h';
        count = 3;
    } else if (i + 2 < input.size() && input[i] == 'a' && input[i + 1] == 't' && input[i + 2] == 'g') {
        result = 'g';
        count = 2;
    } else if (i + 3 < input.size() && input[i] == 's' && input[i + 1] == 'q' && input[i + 2] == 'r' && input[i + 3] == 't') {
        result = 'q';
        count = 3;
    } else if (i + 2 < input.size() && input[i] == 'e' && input[i + 1] == 'x' && input[i + 2] == 'p') {
        result = 'e';
        count = 2;
    } else if (i + 2 < input.size() && input[i] == 'l' && input[i + 1] == 'o' && input[i + 2] == 'g') {
        result = 'l';
        count = 2;
    } else if (i + 4 < input.size() && input[i] == 'l' && input[i + 1] == 'o' && input[i + 2] == 'g' && input[i + 3] == '1' && input[i + 4] == '0') {
        result = 'o';
        count = 4;
    }
    return count;
}

bool tg_check(const vector<vector<double>> &output_arr)
{
    for (const auto &row : output_arr) {
        if (static_cast<TokenType>(row[CODIF]) == TG) { // Приведение к TokenType для сравнения
            return true;
        }
    }
    return false;
}

//07.11.24 Добавлены коментарии, а также изменено: проверка проверки границ, чтобы избежать выхода за пределы строки в is_func.
// А также в priority добавлен знак "^" с приоритетом 3.