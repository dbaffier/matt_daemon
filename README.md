# matt_daemon

A Unix project to create a daemon of type server. This server will listen on a given port and interpret a list of commands.


#### ./Matt_daemon

Here is some informations about Matt_daemon :
  - He should run with root privileges
  - He daemonize himself and run in the background
  - He listen on port 4242
  - He write logs on /var/log/matt_daemon/
  - Only one instance can run at once
  - He lock himself with a file on /var/lock/
  - He handle all catchable signal
  - He is limited to 3 clients
  
 
 #### ./Ben_Afk

Ben_Afk is a shell that connect to the daemon and interpret some commands

Here is some informations about Ben_Afk :
 - There is no password or there is ? you choose
 - List of available commands can be found with help
 - shutdown, shutdown the daemon
 
