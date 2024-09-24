// parser.cpp
#include "parser.h"

#include "class.h"

void parser() {
    string input;
    getline(cin, input);
    unsigned int i = 0;
    unsigned int i_out_arr = 0;
    vector<vector<double>> output_arr;
    output_arr.resize(BUFFER);
    for (int i = 0; i < BUFFER; i++) {
        output_arr[i].resize(2);
    }
    double sum = 0.0;
    while (input[i]) {
        char el = input[i];
        if (isdigit(el)) {
            i += is_digit(i, input, sum, output_arr, i_out_arr);
        } else if (input[i] == 'x') {
            output_arr[i_out_arr][0] = X;
            output_arr[i_out_arr][1] = 0; 
        }
        i++;
    }
    print_arr(output_arr, i_out_arr);
}

void print_arr(vector<vector<double>> &output_arr, unsigned int &i_out_arr) {
    for ( unsigned int i = 0; i < i_out_arr + 1; i++) {
        printf("[i]:%d codif: %.0lf elem: %.2lf\n", i_out_arr, output_arr[i][0], output_arr[i][1]);
    }
    cout << endl;
}

unsigned int is_digit(unsigned int &i, string &input, double &sum, vector<vector<double>> &output_arr, unsigned int &i_out_arr) {
    unsigned int indx = i;
    string double_num = "";
    while (indx < input.size() && (isdigit(input[indx]) || input[indx] == '.')) {
        double_num += input[indx];
        indx++;
    }
    sum = stod(double_num);
    output_arr[i_out_arr][0] = NUM;
    output_arr[i_out_arr][1] = sum;
    sum = 0.0;
    return indx - i - 1;
}
