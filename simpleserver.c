#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#define RECV_BUFSIZE 65536
#define BACKLOG 10


int main(int argc, char** argv) {
    // Step 1: Parse Parameters and initialize variables
    struct addrinfo hints;
    struct addrinfo* info;
    char miniBuff[RECV_BUFSIZE];
    char* port = "60000";

    // Step 2: Find address information of domain and attempt to open socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // OK to use either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //Using TCP
    hints.ai_flags = AI_PASSIVE; // Use my IP (extremely important!!)
    
    getaddrinfo(NULL, port, &hints, &info);
    int sockfd = -1;
    struct addrinfo* node = info;
    while (node != NULL && sockfd == -1) {
        printf("Trying socket\n");
        sockfd = socket(node->ai_family, node->ai_socktype, node->ai_protocol); // NOTE: Not bound to port yet
        if (sockfd == -1) {
            fprintf(stderr, "Error on socket...trying another one\n");
            node = node->ai_next;
        }
        int yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            fprintf(stderr, "Error setting socket options\n");
            exit(1);
        }
        else if (bind(sockfd, node->ai_addr, node->ai_addrlen) == -1) {
            fprintf(stderr, "Error binding socket\n");
            exit(1);
        }
    }
    freeaddrinfo(info);

    // Step 3: Service requests (single threaded for now)
    if (sockfd == -1 || node == NULL) {
        printf("ERROR: Error number %i on opening socket\n", errno);
    }
    else {
        printf("Finished binding. Waiting for connections, sockfd = %i...\n", sockfd);
        if (listen(sockfd, BACKLOG) == -1) {
            fprintf(stderr, "Error listening\n");
        }
        
        struct sockaddr_storage their_addr;
        socklen_t len = sizeof their_addr;
        int clientsockfd = accept(sockfd, (struct sockaddr*)&their_addr, &len);
        printf("Got a connection!  Servicing now...\n");
        fflush(stdout);

        recv(clientsockfd, miniBuff, RECV_BUFSIZE, 0);
        printf("Received: %s\n", miniBuff);
        fflush(stdout);

        char* WELCOME_MSG = "Hello!  Thank you for your message!\n";
        int WELCOME_MSG_LEN = strlen(WELCOME_MSG);
        int bytesSent = send(clientsockfd, WELCOME_MSG, WELCOME_MSG_LEN, 0);
        printf("%i bytes sent (tried to send %i)\n", bytesSent, WELCOME_MSG_LEN);

        close(clientsockfd);
        close(sockfd);
    }
}