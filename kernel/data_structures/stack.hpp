#ifndef STACK_HPP
#define STACK_HPP

template <typename T, int SIZE>
class Stack {
public:
    T data[SIZE];
    int top;
    Stack() : top(-1) {}
    void push(T val) {
        if (top < SIZE - 1)
            data[++top] = val;
    }
    T pop() {
        if (top >= 0)
            return data[top--];
        return T();
    }
    int peek(){
        return top;
    }
    bool empty() const {
        return top == -1;
    }
    int size(){
        return top + 1;
    }
};

#endif
