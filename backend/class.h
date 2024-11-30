#ifndef CLASS_H
#define CLASS_H
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

// Шаблонный класс для реализации стека
template <typename T>
class Stack {
    static const int MSIZE = 10;  // Начальная емкость стека
    vector<T> data;               // Вектор для хранения элементов стека
    int top;                      // Индекс верхнего элемента стека
    int capacity;                 // Текущая емкость стека

   public:
    // Конструктор для инициализации стека
    Stack() {
        capacity = MSIZE;
        data.resize(capacity);
        top = 0;  // Изначально стек пуст
    }

    // Метод для получения текущего размера стека
    int size() const { return top; }

    // Возвращает верхний элемент стека без его удаления
    T peak() {
        if (is_empty()) {
            throw out_of_range("stack is empty");  // Исключение, если стек пуст
        }
        return data[top - 1];
    }

    // Добавление элемента в стек
    void push(const T& elem) {
        if (top == capacity) {
            capacity += MSIZE;
            data.resize(capacity);  // Увеличиваем емкость, если стек полон
        }
        data[top] = elem;
        top++;
    }

    // Удаление элемента из стека
    T pop() {
        if (is_empty()) {
            throw out_of_range("stack is empty");
        }
        T elem = data[top - 1];
        top--;
        // Уменьшаем емкость, если выделено слишком много места
        if (top < capacity - MSIZE && capacity > MSIZE) {
            capacity -= MSIZE;
            data.resize(capacity);
        }
        return elem;
    }

    // Проверка, пуст ли стек
    bool is_empty() const { return top == 0; }

    // Вывод всех элементов стека
    void print_stack() {
        cout << "В стеке: " << endl;
        for (int i = 0; i < top; i++) {
            cout << " [" << i << "]: " << data[i] << endl;
        }
    }

    // Деструктор для очистки данных стека
    ~Stack() { data.clear(); }
};

#endif

// 07.11.24 Последние изменения добавлена читаемость с помощью комментариев