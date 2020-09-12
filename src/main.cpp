#include <iostream>
#include "Daemon.h"

Daemon* Daemon::daemon = 0;

int main(void)
{
	// Daemon::instance()->Daemonize();
	Daemon::instance()->LaunchServer();
	return (0);
}
