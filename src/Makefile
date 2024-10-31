flag=-Wextra -Werror -Wall -std=c++23
GCC = g++

all: build

build: validator.o parser.o test.o calculation.o scope_of_definition.o break_points.o
	$(GCC) -o result validator.o scope_of_definition.o break_points.o parser.o calculation.o test.o

parser.o:
	$(GCC) -c $(flag) parser.cpp

calculation.o:
	$(GCC) -c $(flag) calculation.cpp

test.o:
	$(GCC) -c $(flag) test.cpp

validator.o:
	$(GCC) -c $(flag) validator.cpp

scope_of_definition.o:
	$(GCC) -c $(flag) scope_of_definition.cpp

break_points.o:
	$(GCC) -c $(flag) break_points.cpp

start:
	./result

clang:
	clang-format -i *.h *.cpp

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h *.cpp

rebuild: clean build start

clean:
	rm -rf *.o result

#g++ test_symengine.cpp $(pkg-config --cflags --libs symengine) -lgmp -o your_program
# команда для компилляции  программы с использованием библиотеки SymEngine и GMP