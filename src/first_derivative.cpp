#include "parser.h"

void calculate_first_derivative(vector<vector<double>> &output_arr, unsigned int &i_out_arr, vector<vector<double>> &coordinate_arr, vector<pair<double, double>> &first_derivative) {
    const double INCREMENT = 1e-7;
    for (unsigned int i = 0; i < coordinate_arr.size(); i ++) {
        double x = coordinate_arr[i][X];
        double y = coordinate_arr[i][Y];
        double x_plus_increment = x + INCREMENT;
        double difference = calculator(output_arr, i_out_arr, x_plus_increment);
    }
}