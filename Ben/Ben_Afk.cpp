#include "Ben_Afk.h"

void Client::Connect(void)
{
    if ((ss = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        dprintf(2, "Could not create socket, exiting.\n");
        exit(1);
    }
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY; 
    client_addr.sin_port = inet_addr("127.0.0.0");
    client_addr.sin_port = htons(4242);  /// Server Port
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  /// server ip
    if (connect(ss, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
    {
        dprintf(2, "Could not connect");
        exit(1);
    }
}

int  Client::CheckBuiltin(const char *s)
{
    const char *builtin[2] = { "exit\n", "log\n" }; 

    for (int i = 0; i < 2; i++)
        if (strcmp(builtin[i], s) == 0)
            return (1);
    return (0);
}

void Client::Talk(void)
{
    fd_set rfds;

    struct timeval tv;
    int retval, maxfd;
    while (1)
    {
        /*Clear the collection of readable file descriptors*/
        FD_ZERO(&rfds);
        /*Add standard input file descriptors to the collection*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*Add the currently connected file descriptor to the collection*/
        FD_SET(ss, &rfds);
        /*Find the largest file descriptor in the file descriptor set*/    
        if(maxfd < ss)
            maxfd = ss;
        /*Setting timeout time*/
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        /*Waiting for chat*/
        retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if(retval == -1)
        {
            printf("select Error, client program exit\n");
            break;
        }
        else if(retval == 0)
            continue;
        else
        {
            /*The server sent a message.*/
            if(FD_ISSET(ss, &rfds)){
                char recvbuf[1024];
                int len;
                len = recv(ss, recvbuf, sizeof(recvbuf),0);
                if (len == 0)
                {
                    printf("Connection closed.\n");
                    break ;
                }
                // printf("serv msg : [%s]", recvbuf);
                write(1, recvbuf, len);
                memset(recvbuf, 0, sizeof(recvbuf));
            }
            /*When the user enters the information, he begins to process the information and send it.*/
            if(FD_ISSET(0, &rfds))
            {
                char sendbuf[1024];
                fgets(sendbuf, sizeof(sendbuf), stdin);
                if (CheckBuiltin(sendbuf))
                    printf("Got it \n");
                send(ss, sendbuf, strlen(sendbuf),0); //Send out
                memset(sendbuf, 0, sizeof(sendbuf));
            }
        }
    }
}