#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include <string.h>
#include <errno.h> /* include errno and EINVAL*/

ssize_t custom_getline(char **lineptr, size_t *n) {
    ssize_t read_bytes = 0;
    int c;

    if (lineptr == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (*lineptr == NULL || *n == 0) {
        *n = 128; /* Initial buffer size (adjust as needed)*/
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            return -1;
        }
    }

    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            if (read_bytes > 0) {
                (*lineptr)[read_bytes] = '\0'; /* Null-terminate the line*/
                return read_bytes;
            } else if (c == EOF) {
                return -1; /* No more input*/
            }
        } else {
            if ((size_t)read_bytes + 1 >= *n) {
                size_t new_capacity = *n * 2;
                char *new_line = (char *)realloc(*lineptr, new_capacity);
                if (new_line == NULL) {
                    return -1; /* Error: unable to reallocate*/
                }
                *lineptr = new_line;
                *n = new_capacity;
            }
            (*lineptr)[read_bytes++] = c;
        }
    }
}
