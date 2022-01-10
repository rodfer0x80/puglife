#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "hacking.h"
#include "hacking-network.h"


#define PORT 80 // The port users will be connecting to
#define WEBROOT "webroot" // The web server's root directory


/* This function handles the connection on the passed socket from the
* passed client address. The connection is processed as a web request,
* and this function replies over the connected socket. Finally, the
* passed socket is closed at the end of the function.
*/
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr);
