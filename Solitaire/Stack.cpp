#pragma once
#include <iostream>
#include "List.cpp"

using namespace std;

template <typename T> class Stack {
private:
    DLL<T> stack;
    int size;

public:
    Stack() : size(0) {}

    bool isEmpty() {
        return stack.isEmpty();
    }

    int getSize() {
        return size;
    }

    T getHead() {
        return stack.getHead();
    }

    T pop() {
        T head = stack.getHead();
        stack.deleteHead();
        if (!isEmpty()) --size;
        return head;
    }

    void push(T& element) {
        stack.insertHead(element);
        size++;
    }

    void print() {
        stack.printForward();
    }

    typename DLL<T>::ListIterator begin() {
        return stack.fbegin();
    }

    typename DLL<T>::ListIterator end() {
        return stack.fend();
    }
};
