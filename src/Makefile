flag=-Wextra -Werror -Wall -std=c++11
GCC = g++

all: build

build: validator.o parser.o test.o calculation.o
	$(GCC) -o result validator.o parser.o calculation.o test.o

parser.o:
	$(GCC) -c $(flag) parser.cpp

calculation.o:
	$(GCC) -c $(flag) calculation.cpp

test.o:
	$(GCC) -c $(flag) test.cpp

validator.o:
	$(GCC) -c $(flag) validator.cpp
	

start:
	./result

clang:
	clang-format -i *.h *.cpp

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h *.cpp

rebuild: clean build start

clean:
	rm -rf *.o result
