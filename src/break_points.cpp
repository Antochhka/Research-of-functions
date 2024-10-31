#include "parser.h"

void search_break_points(vector<pair<double, double>> &intervals, vector<pair<double, int>> &break_points,
                  vector<vector<double>> &output_arr, unsigned int &i_out_arr) {
    double EPSILON = 1e-15;
    double INF = 1e+9;
    for (unsigned int i = 0; i < intervals.size() - 1; i++) {
        if (intervals[i].second == intervals[i + 1].first) {
            double break_point = intervals[i].second;
            double left_limit = break_point - EPSILON;
            double right_limit = break_point + EPSILON;
            left_limit = calculator(output_arr, i_out_arr, left_limit);
            right_limit = calculator(output_arr, i_out_arr, right_limit);
            if (left_limit == right_limit && (abs(left_limit) < INF && abs(right_limit) < INF)) {
                break_points.push_back({intervals[i].second, DBP_1});
            } else if (abs(left_limit) < INF && abs(right_limit) < INF) {
                break_points.push_back({intervals[i].second, BP_1});
            } else if (abs(left_limit) >= INF and abs(right_limit) >= INF) {
                break_points.push_back({intervals[i].second, BP_2});                
            }
        }
    }
}

void print_break_points(vector<pair<double, int>> &break_points) {
    for (unsigned int i = 0; i <  break_points.size(); i++) {
        if  (break_points[i].second == DBP_1) {
            printf("[%lf] - устранимая точка разрыва 1 - го рода",  break_points[i].first);
        } else if (break_points[i].second == BP_1) {
            printf("[%lf] - точка разрыва 1 - го рода",  break_points[i].first);           
        } else {
            printf("[%lf] - точка разрыва 2 - го рода", break_points[i].first);
        }
        cout << endl;
    }
}