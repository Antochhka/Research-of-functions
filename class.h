//class.h
#ifndef CLASS_H
#define CLASS_H

#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>

class stack {
    static const int MSIZE = 10;
    vector<T> data;
    int top;
    int size;

   public:
    stack() {
        size = MSIZE;
        data.resize(size);
        top = 0;
    }

    void push(const T& elem) {
        if (top == size) {
            size += MSIZE;
            data.resize(size);
        }
        data[top] = elem;
        top++;
    }

    T pop() {
        if (is_empty()) {
            throw out_of_range("stack is empty");
        }
        if (top < MSIZE && size > MSIZE) {
            size -= MSIZE;
            data.resize(size);
        }
        T elem = data[top - 1];
        top--;
        return elem;
    }

    bool is_empty() const { return top == 0 ? true : false; }

    ~stack() { data.clear(); }
};

#endif 