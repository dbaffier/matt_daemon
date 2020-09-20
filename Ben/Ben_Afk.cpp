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
        dprintf(2, "Could not connect\n");
        exit(1);
    }
}

void Client::Talk(void)
{
    fd_set rfds;

    int retval, maxfd;
    int in = 0;
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
        if (in == 0)
            write(1, "shell> ", 7);
        retval =  select(maxfd + 1, &rfds, NULL, NULL, NULL);
        if(retval == -1)
        {
            printf("select Error, client program exit\n");
            break;
        }
        else
        {
            /*The server sent a message.*/
            if(FD_ISSET(ss, &rfds)){
                char recvbuf[1024];
                int len;
                memset(recvbuf, 0, sizeof(recvbuf));
                while ((len = recv(ss, recvbuf, sizeof(recvbuf) - 1,0)) >= 1023)
                    write(1, recvbuf, len);
                if (len < 0)
                {
                    close(ss);
                    exit(1);
                }
                else if (len == 0)
                {
                    write(1, "Connection closed.\n", 19);
                    break ;
                }
                write(1, recvbuf, len);
                in = 0;
            }
            /*When the user enters the information, he begins to process the information and send it.*/
            if(FD_ISSET(0, &rfds))
            {
                char sendbuf[1024];
                memset(sendbuf, 0, sizeof(sendbuf));
                fgets(sendbuf, sizeof(sendbuf) - 1, stdin);
                if (strcmp(sendbuf, "quit\n") == 0 || strcmp(sendbuf, "quit") == 0)
                    break ;
                send(ss, sendbuf, strlen(sendbuf),0);
                in = 1;
            }
        }
    }
    close(ss);
}
