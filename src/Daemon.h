#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <thread>

class Daemon
{
    public:
        Daemon();
        void Error(std::string err);
        void LaunchServer(void);
        void LaunchClient(int conn);


    private:
        int                 ss;
        int                 conns[3];
        int                 thread;
        int                 opt;
        int                 port;
        struct sockaddr_in  server_addr;
        void Client(int conn);

};