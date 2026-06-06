#pragma once
#include "store.h"

class Server{
    public:
        void start(int port);
    private:
        KeyValueStore kvstore;
        void handleClient(int clientsocket);
};