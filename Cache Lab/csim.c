#include "cachelab.h"
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ASSUMED_LINE_LENGTH 256

void usage() {
    puts(
        "Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
        "Options:\n"
        "  -h            :  Optional help flag that prints usage info\n"
        "  -v            :  Optional verbose flag that displays trace info\n"
        "  -s <num>      :  Number of set index bits (S = 2^s is the number of sets)\n"
        "  -E <num>      :  Associativity (number of lines per set)\n"
        "  -b <num>      :  Number of block bits (B = 2^b is the block size)\n"
        "  -t <filename> :  Name of the `valgrind` trace to replay\n"
    );
    exit(1);
}

char *get_next_line(FILE *fp) {
    char *line = NULL;
    char buffer[ASSUMED_LINE_LENGTH];
    size_t size = 0, len = 0;

    while(
        fgets(buffer, ASSUMED_LINE_LENGTH, fp) != NULL
    ) {
        size_t buffer_len = strlen(buffer);
        if (len + buffer_len + 1 > size) {
            size += ASSUMED_LINE_LENGTH;
            char *temp = realloc(line, size);
            if (temp == NULL) {
                free(line);
                return NULL;
            }

            line = temp;
        }

        strcpy(line + len, buffer);
        len += buffer_len;
        if (buffer[buffer_len - 1] == '\n') {
            break;
        }
    }

    return line;
}

char *lstrip(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }
    
    return s;
}

char *rstrip(char *s) {
    char *back;
    size_t len = strlen(s);

    if (len == 0) {
        return s;
    }

    back = s + len - 1;
    while (back >= s && isspace((unsigned char)*back)) {
        back--;
    }
    
    *(back + 1) = '\0';
    return s;
}

char *strip(char *s) {
    char *trimmed_start = lstrip(s);

    if (trimmed_start != s) {
        memmove(s, trimmed_start, strlen(trimmed_start) + 1);
    }
    
    return rstrip(s);
}

int main(int argc, char *argv[])
{
    int opt;
    bool verbose = false;
    char *trace_file = NULL;

    int s, b, E;
    s = b = E = -1;
    
    while(
        opt = getopt(argc, argv, "hvs:E:b:t:"),
        opt != -1
    ) {
        switch (opt) {
            case 'h':
                usage();
            case 'v':
                verbose = true;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            default:
                printf("%s is not a valid option\n", argv[optind - 1]);
                usage();
        }
    }

    FILE *fp = fopen(trace_file, "r");
    // check required arguments
    if (s == -1 || E == -1 || b == -1 || trace_file == NULL || fp == NULL) {
        printf("Missing required command line argument\n");
        usage();
        exit(1);
    }

    int S = 1 << s; // number of sets
    int B = 1 << b; // block size in bytes
    int hits = 0, misses = 0, evictions = 0;

    

    printSummary(hits, misses, evictions);
    return 0;
}
