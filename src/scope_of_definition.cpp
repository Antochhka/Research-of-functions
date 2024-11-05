#include "parser.h"

void print_intervals(vector<pair<double, double>> &intervals) {
    cout << "### SCOPE OF DEFINITION ###" << endl;
    for (unsigned int i = 0; i < intervals.size(); i++) {
        printf(" [%lf : %lf]\n", intervals[i].first, intervals[i].second);
    }
}

void check_nan_by_range(vector<vector<double>> &output_arr, unsigned int &i_out_arr, vector<bool> &nan_arr) {
    for (unsigned int i = 0; i < nan_arr.size(); i++) {
        double x = LEFT_BORDER + i * SHIFT;
        double res = calculator(output_arr, i_out_arr, x);
        if (std::isnan(res) || std::isinf(res)) {
            nan_arr[i] = false;
        } else {
            nan_arr[i] = true;
        }
    }
}

void create_intervals(vector<bool> &nan_arr, vector<pair<double, double>> &intervals) {
    bool open = false;
    double start = 0.0;
    double const INF = 1e+10;
    double const EPSILON = 8.85 * 1e-12;
    if (nan_arr[0]) {
        start = -INF;
        open = true;
    }
    for (unsigned int i = 1; i < nan_arr.size(); i++) {
        if (open == true && !nan_arr[i]) {
            open = false;
            double end = LEFT_BORDER + i * SHIFT;
            intervals.push_back({start, end});
        } else if (open == false && nan_arr[i]) {
            open = true;
            if (intervals.size() != 0) {
                start = intervals[intervals.size() - 1].second;
            } else {
                start = LEFT_BORDER + i * SHIFT - SHIFT;
                if (fabs(start) <= EPSILON) {
                    start = 0.0;
                }
            }
        }
    }
    if (open) {
        double end = INF;
        intervals.push_back({start, end});
    }
}
