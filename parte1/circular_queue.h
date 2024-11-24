#pragma once
#include <vector>
#include <stdexcept>

class ColaCircular {
private:
    std::vector<int> queue;
    int front, rear;
    int size, capacity; //size es elementos actuales, capacity maximo

    void resize(int new_capacity) {
        std::vector<int> new_queue(new_capacity);
        for (auto i = 0; i < size; i++) {
            new_queue[i] = queue[(front + i) % capacity];
        }
        queue = std::move(new_queue); // corta y pega los elementos de new_queue a queue
        front = 0;
        rear = size;
        capacity = new_capacity;
    }

public:
    
    ColaCircular(int initial_capacity) {
        queue = std::vector<int>(initial_capacity);
        front = 0;
        rear = 0;
        size = 0;
        capacity = initial_capacity;
    }

    void enqueue(int elem) {
        if (size == capacity) { // si se llena duplicar
            resize(2 * capacity);
        }
        queue[rear] = elem;
        rear = (rear + 1) % capacity;
        size++;
    }

    int dequeue() {
        if (size == 0) {
            throw std::out_of_range("cola vacia, no se puede sacar elemento");
        }
        int elem = queue[front];
        front = (front + 1) % capacity;
        size--;

        if (size > 0 && size <= capacity / 4) { //si solo ocupa el 25% se reduce a la mitad
            resize(capacity / 2);
        }
        return elem;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }

    int getCapacity() {
        return capacity;
    }
};