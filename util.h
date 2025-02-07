#ifndef UTIL_H
#define UTIL_H
#include <sys/socket.h>
#include "arraylist.h"


/**
 * @brief Send a message over a socket in chunks until
 * the entire message is sent
 * 
 * @param buff ArrayList buffer containing the message to send
 * @param N number of bytes to send
 * @param sockfd Socket to which to write
 * @return ssize_t Number of bytes sent, or -1 if error
 */
ssize_t sendMessage(char* buff, ssize_t N, ssize_t sockfd) {
    ssize_t sentLen = 0;
    ssize_t thisLen = 0;
    do {
        thisLen = send(sockfd, buff+sentLen, N-sentLen, 0);
        sentLen += thisLen;
    }
    while(sentLen < N && thisLen != -1);
    if (thisLen == -1) {
        sentLen = -1; // There was an error
    }
    return sentLen;
}

/**
 * @brief A memory safe method to scan a line
 * 
 * @param abuf Arraylist buffer in which to append result
 * @param stream File descriptor on which to read the line
 */
void scanLine(ArrayListBuf* abuf, FILE* stream) {
    char c;
    do {
        c = fgetc(stream);
        ArrayListBuf_push(abuf, &c, 1);
    }
    while (c != EOF && c != '\n');
    c = '\0';
    ArrayListBuf_push(abuf, &c, 1);
}

#endif