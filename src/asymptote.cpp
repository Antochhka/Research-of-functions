#include "parser.h"

void search_vertical_asymptote(vector<pair<double, double>> &intervals, vector<vector<double>> &output_arr,
                               unsigned int &i_out_arr, vector<double> &vertical_asymptote) {
    const double EPSILON = 8.85 * 1e-12;
    double INF = 1e+10;
    if (intervals[0].first != -INF || intervals[intervals.size() - 1].second != INF) {
        double point = 0.0;
        if (intervals[0].first != -INF) {
            point = intervals[0].first;
        } else {
            point = intervals[intervals.size() - 1].second;
        }
        double left_limit = point - EPSILON;
        double right_limit = point + EPSILON;
        left_limit = calculator(output_arr, i_out_arr, left_limit);
        right_limit = calculator(output_arr, i_out_arr, right_limit);
        if (!std::isnan(left_limit) || !std::isnan(right_limit)) {
            vertical_asymptote.push_back(point);
        }
    }
    for (unsigned int i = 1; i < intervals.size(); i++) {
        if (intervals[i - 1].second == intervals[i].first) {
            vertical_asymptote.push_back(intervals[i].first);
        }
    }
}

pair<double, double> search_horizontal_asymptote(vector<pair<double, double>> &intervals,
                                                 vector<vector<double>> &output_arr,
                                                 unsigned int &i_out_arr) {
    double INF = 1e+10;
    double k = 0.0;
    double b = 0.0;
    const double ERROR = 228.0;
    if (intervals[0].first == -INF && intervals[intervals.size() - 1].second == INF) {
        k = calculator(output_arr, i_out_arr, INF) / INF;
        bool flag = true;
        if (std::isinf(k)) {
            flag = false;
        }
        INF = 1e+5;
        b = (calculator(output_arr, i_out_arr, INF)) - (k * INF);
        if (b < 0) {
            b = std::floor(b);
        } 
        else {
            b = std::ceil(b);
        }
        if (std::isinf(b)) {
            flag = false;
        }
        printf("k = %lf b = %lf\n", k, b);
        if (!flag) {
            return {ERROR, ERROR};
        }
    }
    return {k, b};
}

void print_vertical_asymptote(vector<double> &vertical_asymptote) {
    cout << "### VERTICAL ASYMPTOTE ###" << endl;
    for (unsigned int i = 0; i < vertical_asymptote.size(); i++) {
        cout << "Vertical asymptote at x = " << vertical_asymptote[i] << endl;
    }
}
