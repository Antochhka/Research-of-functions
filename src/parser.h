// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#define BUFFER 100
#define NUM 0
#define NONE 0
#define X 1

using namespace std;

void parser();
void print_arr(vector<vector<double>> &output_arr, unsigned int &i_out_arr);
unsigned int is_digit(unsigned int &i, string &input, double &sum, vector<vector<double>> &output_arr, unsigned int &i_out_arr);


#endif  // PARSER_H