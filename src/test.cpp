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
        vector<double> vertical_asymptote;
        pair<double, double>  horizontal_asymptote;
        unsigned int i_out_arr = 0;
        output_arr.resize(BUFFER);
        for (int i = 0; i < BUFFER; i++) {
            output_arr[i].resize(2);
        }
        parser(output_arr, i_out_arr, input);
        // bool tg_or_ctg = check_tg_or_ctg(output_arr)

        // for (i < output_arr.size; i ++) {
        //     res = 0;
        //     if (output_arr[CODIF] == TG) {
        //         res 1;
        // }
        // return res;
        int size = RIGHT_BORDER * 2 * 1 / SHIFT + 1;
        nan_arr.resize(size);
        check_nan_by_range(output_arr, i_out_arr, nan_arr);
        create_intervals(nan_arr, intervals);
        calculating_coordinate(output_arr, i_out_arr, coordinate_arr, intervals);
        print_coordinate_arr(coordinate_arr);
        search_break_points(intervals, break_points, output_arr, i_out_arr);
        search_vertical_asymptote(intervals, output_arr, i_out_arr, vertical_asymptote);
        print_intervals(intervals);
        print_break_points(break_points);
        print_vertical_asymptote(vertical_asymptote);
        horizontal_asymptote = search_horizontal_asymptote(intervals, output_arr, i_out_arr);
    }
}