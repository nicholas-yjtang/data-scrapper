#ifndef SERVER_H
#define SERVER_H

class Server {
    public:
        Server(int port = 18080);
        virtual ~Server();
        void start();
        void stop();
    private:
        int port;
};

#endif