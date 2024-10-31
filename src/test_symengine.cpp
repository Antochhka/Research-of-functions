#include <symengine/expression.h>
#include <symengine/functions.h>
#include <symengine/parser.h>

#include <iostream>
#include <string>

using namespace SymEngine;

void find_denominators(const Expression &expr, set_basic &denominators) {
    if (is_a<Mul>(*expr.get_basic())) {  // Ищем умножение на дробь (прямого типа "Div" может не существовать)
        RCP<const Basic> numer, denom;
        auto mul_expr = static_cast<const Mul &>(*expr.get_basic());

        for (const auto &arg : mul_expr.get_args()) {
            if (is_a<Pow>(*arg)) {
                auto pow_expr = rcp_static_cast<const Pow>(arg);
                if (eq(*pow_expr->get_exp(), *minus_one)) {  // Проверяем на степень -1 (обратное)
                    denom = pow_expr->get_base();
                    denominators.insert(denom);
                }
            }
        }
    }

    for (const auto &arg : expr.get_basic()->get_args()) {
        find_denominators(Expression(arg), denominators);  // Рекурсивно проверяем подвыражения
    }
}

int main() {
    std::string input;
    std::cout << "Введите математическую функцию от x: ";
    std::getline(std::cin, input);

    RCP<const Symbol> x = symbol("x");
    set_basic denominators;

    Expression expr;
    try {
        expr = parse(input);
    } catch (const ParseError &e) {
        std::cerr << "Ошибка парсинга: " << e.what() << std::endl;
        return 1;
    }

    find_denominators(expr, denominators);

    std::cout << "Обнаруженные переменные в знаменателе: ";
    for (const auto &d : denominators) {
        std::cout << d->__str__() << " ";
    }
    std::cout << std::endl;

    return 0;
}
