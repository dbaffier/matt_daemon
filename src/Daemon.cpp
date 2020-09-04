#include "Daemon.h"

Daemon::Daemon(void)
{
    opt = 1;
    thread = 0;
    port = 4242;
    if ((ss = socket(AF_INET, SOCK_STREAM, 0)) < 1)
        Error("Socket error.");
    if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        Error("setsockopt error.");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(port);
    if (bind(ss, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        Error("bind error.");
    if (listen(ss, 3) < 0)
        Error("listen error.");

}

void Daemon::Client(int conn)
{
    char        buffer[1024];
    struct timeval tv;
    fd_set      rfds;
    int         retval, maxfd;

    printf("Thread : %d\n", thread);

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        maxfd = 0;
        FD_SET(conn, &rfds);
        if(maxfd < conn)
            maxfd = conn;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1){
            printf("select Error, client program exit\n");
            break;
        }else if(retval == 0){
            printf("The server does not have any input information, and the client does not have any information coming. waiting...\n");
            continue;
        }else{
            /*The client sent a message.*/
            if(FD_ISSET(conn,&rfds)){ 
                memset(buffer, 0 ,sizeof(buffer));
                int len = recv(conn, buffer, sizeof(buffer), 0);
                if(strcmp(buffer, "exit\n") == 0) break;
                printf("%s with %d", buffer, len);
                send(conn, buffer, len, 0); //send back the data to the client
            }
            if(FD_ISSET(0, &rfds)){
                char buf[1024];
                fgets(buf, sizeof(buf), stdin);
                //printf("you are send %s", buf);
                for(int i=0; i<thread; i++) {
                    send(conns[i], buf, sizeof(buf), 0);
                }    
            }
        }
    }
    thread--;
    close(conn);
}

void Daemon::LaunchClient(int conn)
{
    std::thread t(&Daemon::Client, this, conn);
    // t.join();
    t.detach();
    thread++;
    printf("There\n");
}

void Daemon::LaunchServer(void)
{
    int     conn;

    while (1)
    {
        socklen_t   length = sizeof(server_addr);
        conn = accept(ss, (struct sockaddr *)&server_addr, &length);
        if (conn && thread < 3)
            this->LaunchClient(conn);
        printf("Lol");
    }
    close(ss);
}


void Daemon::Error(std::string err)
{
    printf("%s\n", err.c_str());
    exit(1);
}