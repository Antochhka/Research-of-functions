flag = -Wall -Wextra -Werror -std=c++23
GCC = g++
SRC = validator.cpp parser.cpp test.cpp calculation.cpp scope_of_definition.cpp break_points.cpp asymptote.cpp func_period_check.cpp func_sym_check.cpp derivatives.cpp
OBJ = $(SRC:.cpp=.o)

# Добавляем флаги для SymEngine
SYMENGINE_FLAGS = -lsymengine -lgmp

all: build

build: $(OBJ)
	$(GCC) -o result $(OBJ) $(SYMENGINE_FLAGS)

%.o: %.cpp
	$(GCC) -c $(flag) $<

# Отдельное правило для derivatives.o
derivatives.o: derivatives.cpp
	$(GCC) -c $(flag) -I/usr/include/symengine $< $(SYMENGINE_FLAGS)

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
	rm -rf *.o 
