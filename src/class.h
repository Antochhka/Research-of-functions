#ifndef CLASS_H
#define CLASS_H
#include <stdexcept>
#include <vector>

using namespace std;

template <typename T>
class Stack {
    static const int MSIZE = 10;
    vector<T> data;
    int top;
    int capacity;

   public:
    Stack() {
        capacity = MSIZE;
        data.resize(capacity);
        top = 0;
    }

    int size() const { return top; }

    T peak() { return data[top - 1]; }

    void push(const T& elem) {
        if (top == capacity) {
            capacity += MSIZE;
            data.resize(capacity);
        }
        data[top] = elem;
        top++;
    }

    T pop() {
        if (is_empty()) {
            throw out_of_range("stack is empty");
        }
        if (top < MSIZE && capacity > MSIZE) {
            capacity -= MSIZE;
            data.resize(capacity);
        }
        T elem = data[top - 1];
        top--;
        return elem;
    }

    bool is_empty() const { return top == 0; }

    void print_stack() {
        cout << "In stack: " << endl;
        for (int i = 0; i < top; i++) {
            printf(" [%d]: %c ", i, data[i]);
            cout << endl;
        }
    }

    ~Stack() { data.clear(); }
};

#endif
