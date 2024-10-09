// test.cpp
#include "parser.h"

int main() {
    vector<vector<double>> output_arr;
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
        int size = RIGHT_BORDER * 2 * 1 / SHIFT + 1;
        coordinate_arr.resize(size);
        for (int i = 0; i < size; i++) {
            coordinate_arr[i].resize(2);
        }
        calculating_coordinate(output_arr, i_out_arr, coordinate_arr);
        print_coordinate_arr(coordinate_arr);
    }
}