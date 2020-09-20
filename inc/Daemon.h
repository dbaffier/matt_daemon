#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <iostream>
#include <thread>
#include <list>
#include <sys/file.h>
#include <sstream>
#include <signal.h>
#include "Tintin_reporter.h"


class Daemon
{
    public:
        Daemon();
        void                LaunchServer(void);
		void                Daemonize(void);
        void                builtin(const char *s, int ss);
		Tintin_reporter     Log;
        int                 lock(void) { return _lock; }
        static Daemon*      instance() { daemon = (!daemon) ? new Daemon : daemon; return daemon; }



    private:
        int                 ss;

        int                 thread;
        int                 opt;
        int                 port;

		int	                _lock;
    
        struct sockaddr_in  server_addr;
        static Daemon*		daemon;
        
		void getConn(void);
		void sendMess(void);
		void getData(void);
};
