#ifndef DERIVATIVES_H
#define DERIVATIVES_H

#include <symengine/basic.h>
#include <symengine/constants.h>
#include <symengine/derivative.h>
#include <symengine/expression.h>
#include <symengine/functions.h>
#include <symengine/parser.h>
#include <symengine/solve.h>
#include <symengine/symbol.h>
#include <symengine/symengine_exception.h>
#include <symengine/symengine_rcp.h>

#include <iostream>
#include <string>
#include <tuple>

using namespace SymEngine;
using namespace std;

void search_critical_points(vector<vector<double>> &first_critical_points,
                            vector<vector<double>> &second_critical_points,
                            RCP<const Basic> &first_derivative_expr, RCP<const Basic> &second_derivative_expr,
                            string input);
void search_monotony_intervals(vector<pair<double, double>> &intervals,
                               vector<vector<double>> &critical_points,
                               vector<std::tuple<double, double, int>> &monotony_intervals,
                               RCP<const Basic> &first_derivative_expression);
void search_convexity_concavity_intervals(
    vector<pair<double, double>> &intervals, vector<vector<double>> &second_critical_points,
    vector<std::tuple<double, double, int>> &convexity_concavity_intervals,
    RCP<const Basic> &second_derivative_expression);

#endif  // DERIVATIVES_H