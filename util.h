#ifndef UTIL_H
#define UTIL_H
#include <sys/socket.h>
#include "arraylist.h"


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