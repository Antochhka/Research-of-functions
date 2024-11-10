flag = -Wextra -Werror -Wall -std=c++23
GCC = g++
SRC = validator.cpp parser.cpp test.cpp calculation.cpp scope_of_definition.cpp break_points.cpp asymptote.cpp func_period_check.cpp func_sym_check.cpp
OBJ = $(SRC:.cpp=.o)

all: build

build: $(OBJ)
	$(GCC) -o result $(OBJ)

%.o: %.cpp
	$(GCC) -c $(flag) $<

start:
	./result

clang:
	clang-format -i *.h *.cpp

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h *.cpp

git:
	git add *.cpp *.h Makefile
	git commit -m "."
	git push origin Antochhka_develop

rebuild: clean build start

clean:
	rm -rf *.o result