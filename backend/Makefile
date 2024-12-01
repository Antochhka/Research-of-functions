# Флаги компиляции
flag = -Wall -Wextra -Werror -std=c++17
GCC = g++

# Исходные файлы
SRC = validator.cpp parser.cpp calculation.cpp scope_of_definition.cpp break_points.cpp asymptote.cpp func_period_check.cpp func_sym_check.cpp derivatives.cpp Server.cpp
OBJ = $(SRC:.cpp=.o)

# Флаги для SymEngine
SYMENGINE_FLAGS = -lsymengine -lgmp

# Цель по умолчанию
all: build

# Цель для сборки
build: $(OBJ)
	$(GCC) -o server.exe $(OBJ) $(SYMENGINE_FLAGS)

# Правило для компиляции объектных файлов
%.o: %.cpp
	$(GCC) -c $(flag) $<

# Отдельное правило для derivatives.o
derivatives.o: derivatives.cpp
	$(GCC) -c $(flag) -I/usr/include/symengine $< $(SYMENGINE_FLAGS)

# Запуск программы
start:
	./server.exe

# Форматирование кода с помощью clang-format
clang:
	clang-format -i *.h *.cpp

# Проверка кода с помощью cppcheck
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h *.cpp

# Добавление файлов в git, коммит и push
git:
	git add *.cpp *.h Makefile
	git commit -m "."
	git push origin Antochhka_develop

# Пересборка проекта
rebuild: clean build start

# Очистка объектных файлов
clean:
	rm -rf *.o server.exe
