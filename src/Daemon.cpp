#include "Daemon.h"

std::list<int>		lst;

static void sig_handler(int sig)
{
    Daemon::instance()->Log.Log(LOG_SIG, std::string("Caught signal [ ") + strsignal(sig) + std::string(" ]."));
    Daemon::instance()->Log.Log(LOG_INFO, "Quitting.");
    close(Daemon::instance()->lock());
    remove("/var/lock/matt_daemon.lock");
    exit(128 + sig);
}

Daemon::Daemon(void)
{
    opt = 1;
    thread = 0;
    port = 4242;
}

void Daemon::getData()
{
	struct timeval tv;

    tv.tv_sec = 2;
    tv.tv_usec = 0;
    while(1)
	{
        std::list<int>::iterator it;
        for (it = lst.begin(); it != lst.end(); ++it){
            fd_set rfds;
            FD_ZERO(&rfds);
            int maxfd = 0;
            int retval = 0;
            FD_SET(*it, &rfds);
            if(maxfd < *it){
                maxfd = *it;
            }
            retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
            if(retval == -1){
                printf("select error\n");
            }else if(retval == 0) {
                printf("not message\n");
            }else{
              char buf[1024];
                memset(buf, 0 ,sizeof(buf));
                int len = recv(*it, buf, sizeof(buf), 0);
                // if (len)
                    // printf("%s\n", buf);
                write(1, buf, len);
                (void)len;
            }
        }
        sleep(1);
    }
}

void Daemon::getConn()
{
    while(1)
	{
        socklen_t   length = sizeof(server_addr);
        int conn = accept(ss, (struct sockaddr*)&server_addr, &length);
        lst.push_back(conn);
        // printf("%d\n", conn);
    }
}

void Daemon::sendMess()
{
    while(1)
	{
        char buf[1024];
        memset(buf, 0 ,sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        std::list<int>::iterator it;
        for(it = lst.begin(); it != lst.end(); ++it){
            send(*it, buf, sizeof(buf), 0);
        }
    }
}

void Daemon::LaunchServer(void)
{

    Log.Log(LOG_INFO, "Creating server.");
    if ((ss = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Log.Log(LOG_ERR, "Cannot create socket");
        close(_lock);
        remove("/var/lock/matt_daemon.lock");
        exit(1);
    }
    if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        Log.Log(LOG_ERR, "setsockopt error.");
        close(_lock);
        remove("/var/lock/matt_daemon.lock");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(port);
    if (bind(ss, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        Log.Log(LOG_ERR, "Cannot bind socket.");
        close(_lock);
        remove("/var/lock/matt_daemon.lock");
        exit(1);
    }
    if (listen(ss, 3) < 0)
    {
        Log.Log(LOG_ERR, "Cannot listen (3).");
        close(_lock);
        remove("/var/lock/matt_daemon.lock");
        exit(1);
    }
	std::thread t(&Daemon::getConn, this);
	t.detach();
	std::thread t1(&Daemon::sendMess, this);
	t1.detach();
	std::thread t2(&Daemon::getData, this);
    t2.detach();
    Log.Log(LOG_INFO, "Server created.");
    while (1)
		;
}

void Daemon::Daemonize(void)
{
	//	https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux/17955149#17955149
	pid_t	pid;

    Log.Log(LOG_INFO, "Entering daemon mode.");
	pid = fork();
	if (pid < 0)
		exit(1);
	if (pid > 0)
		exit(0);
	if (setsid() < 0)
		exit(1);
	//TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

	pid = fork();
	if (pid < 0)
        exit(1);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(0);
	    /* Set new file permissions */
    umask(0);
    
    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");
    _lock = open("/var/lock/matt_daemon.lock", O_CREAT, 0644);
    if (_lock < 0)
    {
        Log.Log(LOG_ERR, "Can't open :/var/lock/matt_daemon.lock");
        Log.Log(LOG_INFO, "Quitting.");        exit(1);
    }
    if (flock(_lock, LOCK_EX | LOCK_NB) < 0)
    {
        Log.Log(LOG_ERR, "Error file locked");
        Log.Log(LOG_INFO, "Quitting.");
        dprintf(2, "Can't open :/var/lock/matt_daemon.lock\n");
        close(_lock);
        exit(1);
    }
    close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
    int i = 0;
    while (++i < 32)
        signal(i, &sig_handler);
    Log.Log(LOG_INFO, "started. PID:" + std::to_string(getpid()));
}
