#include "puglife.h"

void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr) {
    unsigned char *ptr, request[500], resource[500];
    int fd, length;

    length = recv_line(sockfd, request);
    printf("Got request from %s:%d\n\t \"%s\"\n", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);
    ptr = strstr(request, " HTTP/"); // Search for valid-looking request.
    if(ptr == NULL) { // Then this isn't valid HTTP.
        printf("NOT HTTP\\n");
    } else {
        *ptr = 0; // Terminate the buffer at the end of the URL.
        ptr = NULL; // Set ptr to NULL (used to flag for an invalid request).
        if(strncmp(request, "GET ", 4) ==0) // GET request
            ptr = request+4; // ptr is the URL.
        if(strncmp(request, "HEAD ", 5) == 0) // HEAD request
            ptr = request+5; // ptr is the URL.
        if(strncmp(request, "POST ", 5) == 0){ // HEAD request
            ptr = request+5; // ptr is the URL.
            // write data to disk in tmp.txt file
            FILE *fp;
            fp = fopen("tmp.txt", "w");
            if(fp == NULL)
                fatal("tempfile can't be opened\n");
            fprintf(fp, "%s", ptr);
            fclose(fp);
            send_string(sockfd, "HTTP/1.1 502 BAD GATEWAY\r\n"); // try to stealth trick IDS 
            // we safely write the data to a file using a function
            // someone much smarter already built before
            // and then use a scripting tool like python to
            // avoid having to worry with any funny formatting business
            // without even having to restart the server and with a lot less code
            // call script to format and write new data to markdown page
            system("./write_grabz.py");
        } else {
            if(ptr == NULL) { // Then this is not a recognized request.
                printf("\t\t UNKNOWN REQUEST\n");
            } else { // Valid request, with ptr pointing to the resource name
                if (ptr[strlen(ptr) - 1] =='/') // For resources ending with '/,
                    strcat(ptr, "index.html");  // add ‘indexhtml' to the end.
                strcpy(resource, WEBROOT); // Begin resource with web root path
                strcat(resource, ptr); // and join it with resource path.
                printf("\t\tOpening %s", resource);
                if(fd == -1) { // If file is not found
                    printf(" :: 404 Not Found\n");
                    send_string(sockfd, "HTTP/1.1 404 NOT FOUND\r\n");
                    send_string(sockfd, "Server: Puglife webserver\r\n\r\n");
                    send_string(sockfd, "<html><head><title>404 Not Found</title></head>");
                    send_string(sockfd, "<body><h1>URL not found</n1></body></htm|>\r\n");
                } else {  //Otherwise, serve up the file.
                    printf(" :: 200 OK\n");
                    send_string(sockfd, "HTTP/1.1 200 OK\r\n");
                    send_string(sockfd, "Server: Puglife webserver\r\n\r\n");
                    if(ptr == request + 4) {// Then this is a GET request
                        if((length = get_file_size(resource)) == -1)
                            fatal("getting resource file size");
                        if((ptr = (unsigned char *) ec_malloc(length)) == NULL)
                            fatal("allocating memory for reading resource");
                        ptr = read_whole_file(resource); // Read the file into memory.
                        send(sockfd, ptr, length, 0); // Send it to socket.
                        free(ptr); // Free file memory.
                    }
                } // End if block for file found/not found.
            } // End if block for valid request.
        } // End POST request drop
    } // End if block for valid HTTP.
    if (shutdown(sockfd, 2) == -1) // Close the socket gracefully.
        fatal("closing connection");
}


int main(void) {
    int sockfd, new_sockfd, yes=1;
    struct sockaddr_in host_addr, client_addr; // My address information
    socklen_t sin_size;

    clear();
    printf("Accepting web requests on port %d\n", PORT);

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        fatal("in socket");

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        fatal("Setting socket option SOQ. REUSEADDR'");

    host_addr.sin_family=AF_INET; // Host byte order
    host_addr.sin_port=htons(PORT); // Short, network byte order
    host_addr.sin_addr.s_addr = INADDR_ANY; // Automatically fill with my IP.
    memset(&(host_addr.sin_zero), '\0', 8); // Zero the rest of the struct.

    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
        fatal("binding to socket");

    if (listen(sockfd, 20) == -1)
        fatal("listening on socket");

    while(1) { //Acceptloop.
        sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if(new_sockfd == -1)
            fatal("accepting connection");
        handle_connection(new_sockfd, &client_addr);
    }
    return 0;
}
