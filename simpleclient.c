#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAGIC_BUFLEN 10

int main(int argc, char** argv) {
    // Step 1: Setup file descriptor for socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sockfd = %i\n", sockfd);
    if (sockfd == -1) {
        fprintf(stderr, "Error number %i opening socket\n", errno);
        exit(errno);
    }
    // Step 2: Setup stream on socket and connect
    struct sockaddr_in addrInfo;
    addrInfo.sin_family = AF_INET;
    addrInfo.sin_port = htons(60000);
    inet_pton(AF_INET, "127.0.0.1", &addrInfo.sin_addr.s_addr);
    int ret = connect(sockfd, (struct sockaddr*)&addrInfo, sizeof(addrInfo));
    if (ret == -1) {
        fprintf(stderr, "Error number %i connecting to socket\n", errno);
        exit(errno);
    }
    // Step 3: Send message
    char* message = "Hello remote person 475!\n";
    int len = strlen(message) + 1;
    int bytesSent = send(sockfd, message, len, 0);
    printf("%i bytes sent (tried to send %i)\n", bytesSent, len);
    
    char payload[MAGIC_BUFLEN];
    int receivedBytes = recv(sockfd, payload, MAGIC_BUFLEN,0);
    printf("Message received: %s\n", payload);
    printf("receviedBytes = %i\n", receivedBytes);

    close(sockfd);
}