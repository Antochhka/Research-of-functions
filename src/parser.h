// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#define BUFFER 100
#define NONE 0
#define NUM 0
#define X 1
#define PLUS 2
#define MINUS 3
#define MULT 4
#define DIV 5
#define SIN 6
#define COS 7
#define TG 8
#define ASIN 9
#define ACOS 10
#define ATG 11
#define SQRT 12
#define EXP 13
#define LOG 14
#define LOG2 15
#define LOG10 16
#define POW 17

using namespace std;

void parser();
void print_arr(vector<vector<double>> &output_arr, unsigned int &i_out_arr);
unsigned int is_digit(unsigned int &i, string &input, double &sum, vector<vector<double>> &output_arr,
                      unsigned int &i_out_arr);
unsigned int priority(char &el);

#endif  // PARSER_H