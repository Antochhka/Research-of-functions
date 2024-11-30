#include "validator.h"

#include <cctype>
#include <iostream>
#include <locale>
#include <regex>
#include <stack>
#include <string>

#include "analysis.h"
#include "calculation.h"
#include "parser.h"

// Функция для проверки правильности использования скобок
bool checkBrackets(const std::string& expression, std::string& error) {
    std::stack<int> bracketStack;
    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];
        if (c == '(') {
            bracketStack.push(static_cast<int>(i));
        } else if (c == ')') {
            if (bracketStack.empty()) {
                error = "Закрывающая скобка без открывающей в позиции: " + std::to_string(i);
                return false;
            }
            bracketStack.pop();
        }
    }
    if (!bracketStack.empty()) {
        error = "Открывающая скобка без закрывающей в позиции: " + std::to_string(bracketStack.top());
        return false;
    }
    return true;
}

// Функция для удаления пробелов из строки
std::string removeSpaces(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
    return result;
}

// Функция для проверки валидных символов
bool checkValidCharacters(const std::string& expression, std::string& error) {
    try {
        std::regex validRegex(R"([0-9()+\-*/^a-zA-Z\.]+)");
        if (!std::regex_match(expression, validRegex)) {
            error = "Недопустимые символы в строке";
            return false;
        }
    } catch (const std::regex_error& e) {
        error = "Ошибка в регулярном выражении: " + std::string(e.what());
        return false;
    }
    return true;
}

// Функция для проверки правильного использования операторов
bool checkOperators(const std::string& expression, std::string& error) {
    std::string operators = "+-*/^";
    bool lastWasOperator = true;  // Начинаем с true для предотвращения строки,
                                  // начинающейся с оператора

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (operators.find(c) != std::string::npos) {
            // Специальный случай: оператор '-' допустим перед переменной или функцией
            // (например, -x или sin(-x))
            if (c == '-' && (i == 0 || expression[i - 1] == '(')) {
                lastWasOperator = false;
                continue;
            }
            // Если предыдущий символ был оператором, то текущий оператор использовать
            // нельзя
            if (lastWasOperator) {
                error = "Неправильное использование оператора в позиции: " + std::to_string(i);
                return false;
            }
            // Обновляем флаг, так как текущий символ — оператор
            lastWasOperator = true;
        } else if (c == '(' || c == ')') {
            // Обновляем флаг в зависимости от того, открывающая или закрывающая
            // скобка
            lastWasOperator = (c == '(');
        } else {
            // Если текущий символ не оператор, сбрасываем флаг
            lastWasOperator = false;
        }
    }
    if (lastWasOperator) {
        error = "Строка заканчивается оператором.";
        return false;
    }
    return true;
}

// Функция для проверки наличия операндов в строке
bool checkOperands(const std::string& expression, std::string& error) {
    std::regex operandRegex("[0-9a-zA-Z]");
    if (!std::regex_search(expression, operandRegex)) {
        error = "В строке отсутствуют операнды.";
        return false;
    }
    return true;
}

// Функция для проверки корректности имен функций (например, sin, cos и т.д.)
bool checkFunctionNames(const std::string& expression, std::string& error) {
    static const std::vector<std::string> validFunctions = {
        "sin", "cos", "tan", "exp", "ln", "asin", "acos", "atg", "sqrt", "log",
    };
    std::regex functionCallRegex("[a-zA-Z]+\\(");  // Регулярное выражение для поиска возможных вызовов
                                                   // функций
    std::sregex_iterator words_begin(expression.begin(), expression.end(), functionCallRegex);
    std::sregex_iterator words_end;

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string match = (*i).str();
        // Удаляем '(' для проверки функции
        match.pop_back();
        if (std::find(validFunctions.begin(), validFunctions.end(), match) == validFunctions.end()) {
            error = "Некорректное имя функции: " + match;
            return false;
        }
    }
    return true;
}

// Основная функция-валидатор
bool validateMathExpression(const std::string& expression) {
    std::string error;
    // Удаляем все пробелы из строки перед началом проверки
    std::string cleanedExpression = removeSpaces(expression);

    // Проверка на пустую строку
    if (cleanedExpression.empty()) {
        std::cout << "Ошибка: Пустая строка." << std::endl;
        return false;
    }

    // Проверка на правильность скобок
    if (!checkBrackets(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    // Проверка на допустимые символы
    if (!checkValidCharacters(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    // Проверка на правильное использование операторов
    if (!checkOperators(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    // Проверка на наличие операндов в строке
    if (!checkOperands(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    // Проверка корректности имен функций
    if (!checkFunctionNames(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    std::cout << "Строка корректна." << std::endl;
    return true;
}

// 24.11.24 Добавлена читаемость и комментарии