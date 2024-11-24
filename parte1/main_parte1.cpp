#include <iostream>
#include <thread>
#include <string.h>
#include "monitor.h"

void producer(Monitor& monitor, int id, int elems) {
    monitor.addProductor();
    for (int i = 0; i < elems; i++) {
        monitor.enqueue(id * 100 + i);
        std::cout << "Productor " << id << " agrego " << id * 100 + i << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Productor " << id << " termino y el jhos callampin bombin" << "\n";
    monitor.removeProductor();
}

void consumer(Monitor& monitor, int id, int wait_time) {
    int elem;
    auto end_time = std::chrono::steady_clock::now() + std::chrono::seconds(wait_time);
    while (std::chrono::steady_clock::now() < end_time) {
        if (!monitor.dequeue(elem)) {
            break;
        }
        std::cout << "Consumidor " << id << " saco " << elem << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Consumidor " << id << " termino y el jhos callampin bombin" << "\n";
}

int main(int argc, char* argv[]) {

    const char* args[] = {"-p", "-c", "-s", "-t"};
    int productores, consumidores;
    int queue_capacity, seconds;
    if (argc != 9) {
        std::cout << "Uso: " << argv[0] << " -p <productores> -c <consumidores> -s <capacidad> -t <segundos>\n";
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], args[0]) == 0) {
            productores = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], args[1]) == 0) {
            consumidores = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], args[2]) == 0) {
            queue_capacity = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], args[3]) == 0) {
            seconds = atoi(argv[i + 1]);
        }
    }

    Monitor monitor(queue_capacity);
    std::vector<std::thread> threads;
    for (int i = 0; i < productores; i++) {
        threads.push_back(std::thread(producer, std::ref(monitor), i, seconds));
    }
    for (int i = 0; i < consumidores; i++) {
        threads.push_back(std::thread(consumer, std::ref(monitor), i, seconds));
    }
    for (auto& t : threads) {
        t.join();
    }


    return 0;
}