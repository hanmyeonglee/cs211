#include "cachelab.h"
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int lru_timestamp = 0;

#define ASSUMED_LINE_LENGTH 256

bool verbose = false;
int s = -1, E = -1, b = -1;
int hits = 0, misses = 0, evictions = 0;

typedef unsigned long addr_t;

typedef struct {
    bool valid;
    addr_t tag;
    int timestamp;
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

char *access(
    cache_t cache,
    addr_t set_index,
    addr_t tag
) {
    lru_timestamp++;

    cache_set_t set = cache[set_index];
    for (int i = 0; i < E; i++) {
        if (set[i].valid && set[i].tag == tag) {
            hits++;
            
            set[i].timestamp = lru_timestamp;
            return "hit";
        }
    }
    
    misses++;
    for (int i = 0; i < E; i++) {
        if (!set[i].valid) {
            set[i].valid = true;
            set[i].tag = tag;
            set[i].timestamp = lru_timestamp;
            return "miss";
        }
    }
    
    evictions++;
    int lru_index = 0;
    for (int i = 1; i < E; i++) {
        if (set[i].timestamp < set[lru_index].timestamp) {
            lru_index = i;
        }
    }

    set[lru_index].tag = tag;
    set[lru_index].timestamp = lru_timestamp;
    return "miss eviction";
}

int main(int argc, char *argv[])
{
    int opt;
    char *trace_file = NULL;
    
    while(
        opt = getopt(argc, argv, "hvs:E:b:t:"),
        opt != -1
    ) {
        switch (opt) {
            case 'h':
                usage();
                break;
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
                break;
        }
    }

    // check required arguments
    if (s == -1 || E == -1 || b == -1 || trace_file == NULL) {
        printf("Missing required command line argument\n");
        usage();
    }

    if (s > 64 - b) {
        printf("Invalid arguments: s + b must not exceed 64\n");
        usage();
    }

    FILE *fp = fopen(trace_file, "r");
    if (fp == NULL) {
        printf("Error: Could not open trace file '%s'\n", trace_file);
        exit(1);
    }

    int S = 1 << s;
    int B = 1 << b;

    cache_t cache = malloc(S * sizeof(cache_set_t));
    for (int i = 0; i < S; i++) {
        cache[i] = malloc(E * sizeof(cache_line_t));
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = false;
            cache[i][j].tag = 0;
            cache[i][j].timestamp = lru_timestamp;
        }
    }

    char line[ASSUMED_LINE_LENGTH];
    while (
        fgets(line, ASSUMED_LINE_LENGTH, fp) != NULL
    ) {
        char *copied_line = strdup(line);
        char *stripped_line = strip(line);
        char op = 0;
        addr_t addr = 0, size = 0, cnt = 0;

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
                    size = (addr_t)strtoul(token, NULL, 10);
                    break;
            }
            cnt++;
            token = strtok(NULL, " ,");
        }

        if (
            (op != 'L' && op != 'S' && op != 'M' && op != 'I') ||
            cnt != 3
        ) {
            printf("Invalid trace line: %s\n", copied_line);
            exit(1);
        }

        if (op == 'I') continue;

        addr_t set_index = (addr >> b) & (S - 1);
        addr_t tag = addr >> (b + s);

        char *result = access(cache, set_index, tag);
        if (op == 'M') {
            char *result2 = access(cache, set_index, tag);
            if (verbose) {
                printf("%c %lx,%lu %s %s\n", op, addr, size, result, result2);
            }
        } else {
            if (verbose) {
                printf("%c %lx,%lu %s\n", op, addr, size, result);
            }
        }
    }

    printSummary(hits, misses, evictions);

    for (int i = 0; i < S; i++) {
        free(cache[i]);
    }
    free(cache);
    fclose(fp);
    
    return 0;
}
