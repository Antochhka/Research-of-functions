// test.cpp
#include "parser.h"

int main() {
    string input;
    getline(cin, input);
    if (validateMathExpression(input)) {
        vector<vector<double>> output_arr;
        vector<bool> nan_arr;
        vector<pair<double, double>> intervals;
        vector<pair<double, int>> break_points;
        vector<vector<double>> coordinate_arr;
        unsigned int i_out_arr = 0;
        output_arr.resize(BUFFER);
        for (int i = 0; i < BUFFER; i++) {
            output_arr[i].resize(2);
        }
        parser(output_arr, i_out_arr, input);
        int size = RIGHT_BORDER * 2 * 1 / SHIFT + 1;
        nan_arr.resize(size);
        check_nan_by_range(output_arr, i_out_arr, nan_arr);
        create_intervals(nan_arr, intervals);
        print_intervals(intervals);
        calculating_coordinate(output_arr, i_out_arr, coordinate_arr, intervals);
        print_coordinate_arr(coordinate_arr);
        search_break_points(intervals, break_points, output_arr, i_out_arr);
        print_break_points(break_points);

    }
}