#include "cachelab.h"
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ASSUMED_LINE_LENGTH 256

typedef unsigned long ul_t;

typedef struct {
    int valid;
    ul_t tag;
    int lru_counter;
} cache_line_t;

typedef cache_line_t* cache_set_t;
typedef cache_set_t* cache_t;

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

void access_cache(cache_t cache, ul_t block_offset, ul_t set_index, ul_t tag) {
    // To be implemented
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
    if (
        s == -1 || E == -1 || b == -1 || 
        trace_file == NULL || fp == NULL ||
        s > 64 - b
    ) {
        printf("Missing required command line argument\n");
        usage();
        exit(1);
    }

    int S = 1 << s;
    int B = 1 << b;
    int hits = 0, misses = 0, evictions = 0;

    cache_t cache = malloc(S * sizeof(cache_set_t));
    for (int i = 0; i < S; i++) {
        cache[i] = malloc(E * sizeof(cache_line_t));
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].lru_counter = 0;
        }
    }

    char line[ASSUMED_LINE_LENGTH];
    while (
        fgets(line, ASSUMED_LINE_LENGTH, fp) != NULL
    ) {
        char *stripped_line = strip(line);
        char op = 0;
        ul_t addr = 0, size = 0, cnt = 0;

        char *token = strtok(stripped_line, " ,");
        while (token != NULL && cnt < 3) {
            switch (cnt) {
                case 0:
                    op = token[0];
                    break;
                case 1:
                    addr = strtoul(token, NULL, 16);
                    break;
                case 2:
                    size = (ul_t)atoi(token);
                    break;
            }
            cnt++;
            token = strtok(NULL, " ,");
        }

        if (cnt != 3) {
            free(line);
            printf("Invalid trace line: %s\n", line);
            exit(1);
        }

        if (op == 'I') continue;

        ul_t block_offset = addr & (B - 1);
        ul_t set_index = (addr >> b) & (S - 1);
        ul_t tag = addr >> (b + s);

        access_cache(cache, block_offset, set_index, tag);

        free(line);
    }

    printSummary(hits, misses, evictions);
    return 0;
}
