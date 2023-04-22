#ifndef SERVER_H
#define SERVER_H

class Server {
    public:
        Server();
        virtual ~Server();
        void start();
        void stop();
};

#endif