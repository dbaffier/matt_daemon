#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>

class Client
{
    public:
        void    Connect(void);
        void    Talk(void);
        int     CheckBuiltin(const char *string);

    private:
        int ss;

        struct sockaddr_in  client_addr;

};