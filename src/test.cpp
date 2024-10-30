// test.cpp
#include "parser.h"

int main() {
    vector<vector<double>> output_arr;
    vector<pair<double, double>> intervals;
    vector<bool> nan_arr;
    int size = RIGHT_BORDER * 2 * 1 / SHIFT + 1;
    nan_arr.resize(size);
    unsigned int i_out_arr = 0;
    output_arr.resize(BUFFER);
    for (int i = 0; i < BUFFER; i++) {
        output_arr[i].resize(2);
    }
    string input;
    getline(cin, input);
    if (validateMathExpression(input)) {
        parser(output_arr, i_out_arr, input);
        vector<vector<double>> coordinate_arr;
        check_nan_by_range(output_arr, i_out_arr, nan_arr);
        create_intervals(nan_arr, intervals);
        print_intervals(intervals);
        calculating_coordinate(output_arr, i_out_arr, coordinate_arr, intervals);
        print_coordinate_arr(coordinate_arr);
    }
}