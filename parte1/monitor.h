#pragma once
#include <mutex>
#include <condition_variable>
#include "circular_queue.h"

class Monitor {
private:
    ColaCircular queue;
    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    int productores_activos;

public:

    Monitor(int initial_capacity) : queue(initial_capacity) {
        productores_activos = 0;
    }

    void addProductor() {
        std::unique_lock<std::mutex> lock(mtx);
        productores_activos++;
    }

    void removeProductor() {
        std::unique_lock<std::mutex> lock(mtx);
        productores_activos--;
        if (productores_activos == 0) {
            not_empty.notify_all();
        }
    }

    void enqueue(int item) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [this]() { return queue.getSize() < queue.getCapacity(); });

        queue.enqueue(item);
        not_empty.notify_one();
    }

    bool dequeue(int& item) {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this]() { return !queue.isEmpty() || productores_activos == 0; });

        if (queue.isEmpty() && productores_activos == 0) {
            return false; // no mas elementos y no hay productores activos
        }

        item = queue.dequeue();
        not_full.notify_one();
        return true;
    }

};
