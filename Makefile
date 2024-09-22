flag=-Wextra -Werror -Wall -std=c++11
GCC = g++

all: build

build: parser.o test.o
	$(GCC) -o result parser.o test.o

parser.o:
	$(GCC) -c $(flag) parser.cpp

test.o:
	$(GCC) -c $(flag) test.cpp
