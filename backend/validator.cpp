#include <cctype>
#include <iostream>
#include <locale>
#include <regex>
#include <stack>
#include <string>

// Функция для проверки правильности использования скобок
bool checkBrackets(const std::string& expression, std::string& error) {
    std::stack<int> bracketStack;
    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];
        if (c == '(') {
            bracketStack.push(i);
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
    bool lastWasOperator = true;  // Начинаем с true для предотвращения строки, начинающейся с оператора

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (operators.find(c) != std::string::npos) {
            if (lastWasOperator) {
                error = "Неправильное использование оператора в позиции: " + std::to_string(i);
                return false;
            }
            lastWasOperator = true;
        } else if (c == '(' || c == ')') {
            lastWasOperator = (c == '(');
        } else {
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

// Основная функция-валидатор
bool validateMathExpression(const std::string& expression) {
    std::string error;
    std::string cleanedExpression = removeSpaces(expression);

    if (cleanedExpression.empty()) {
        std::cout << "Ошибка: Пустая строка." << std::endl;
        return false;
    }

    if (!checkBrackets(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    if (!checkValidCharacters(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    if (!checkOperators(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    if (!checkOperands(cleanedExpression, error)) {
        std::cout << "Ошибка: " << error << std::endl;
        return false;
    }

    std::cout << "Строка корректна." << std::endl;
    return true;
}