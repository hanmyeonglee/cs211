#include "cachelab.h"
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

void usage() {
    puts(
        "Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
        "Options:\n"
        "  -h               Optional help flag that prints usage info\n"
        "  -v               Optional verbose flag that displays trace info\n"
        "  -s <num>         Number of set index bits (S = 2^s is the number of sets)\n"
        "  -E <num>         Associativity (number of lines per set)\n"
        "  -b <num>          Number of block bits (B = 2^b is the block size)\n"
        "  -t <filename>    Name of the `valgrind` trace to replay\n"
    );
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
                exit(1);
        }
    }

    FILE *file = fopen(trace_file, "r");
    // check required arguments
    if (s == -1 || E == -1 || b == -1 || trace_file == NULL || file == NULL) {
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
