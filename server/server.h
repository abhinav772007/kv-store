#pragma once
#include "store.h"
#include<thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>

class Server{
    public:
        void start(int port);
    private:
        KeyValueStore kvstore;
        void handleClient(int clientsocket);
        void workerThread();

        std::vector<std::thread> connections;
        std::queue<int> tasks;
        std::mutex queuemutex;
        std::condition_variable cv;
};