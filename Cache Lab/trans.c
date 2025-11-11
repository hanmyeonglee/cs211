/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, cblock, rblock, v0, v1, v2, v3, v4, v5, v6, v7;
    if (M == 61 && N == 67) {
        for (cblock = 0 ; cblock < 61 ; cblock += 17) {
            for (rblock = 0 ; rblock < 67 ; rblock += 17) {
                for (i = rblock ; i < rblock + 17 && i < 67 ; i++) {
                    for (j = cblock ; j < cblock + 17 && j < 61 ; j++) {
                        v0 = A[i][j];
                        B[j][i] = v0;
                    }
                }
            }
        }
    } else if (M == 32 && N == 32) {
        for (cblock = 0 ; cblock < 32 ; cblock += 8) {
            for (rblock = 0 ; rblock < 32 ; rblock += 8) {
                for (i = rblock ; i < rblock + 8 ; i++) {
                    v0 = A[i][cblock];
                    v1 = A[i][cblock + 1];
                    v2 = A[i][cblock + 2];
                    v3 = A[i][cblock + 3];
                    v4 = A[i][cblock + 4];
                    v5 = A[i][cblock + 5];
                    v6 = A[i][cblock + 6];
                    v7 = A[i][cblock + 7];

                    B[cblock][i] = v0;
                    B[cblock + 1][i] = v1;
                    B[cblock + 2][i] = v2;
                    B[cblock + 3][i] = v3;
                    B[cblock + 4][i] = v4;
                    B[cblock + 5][i] = v5;
                    B[cblock + 6][i] = v6;
                    B[cblock + 7][i] = v7;
                }
            }
        }
    } else { // M == 64 and N == 64
        for (cblock = 0 ; cblock < 64 ; cblock += 8) {
            for (rblock = 0 ; rblock < 64 ; rblock += 8) {
                for (i = rblock ; i < rblock + 4 ; i++) {
                    v0 = A[i][cblock];
                    v1 = A[i][cblock + 1];
                    v2 = A[i][cblock + 2];
                    v3 = A[i][cblock + 3];
                    v4 = A[i][cblock + 4];
                    v5 = A[i][cblock + 5];
                    v6 = A[i][cblock + 6];
                    v7 = A[i][cblock + 7];

                    B[cblock][i] = v0;
                    B[cblock + 1][i] = v1;
                    B[cblock + 2][i] = v2;
                    B[cblock + 3][i] = v3;
                    B[cblock][i + 4] = v4;
                    B[cblock + 1][i + 4] = v5;
                    B[cblock + 2][i + 4] = v6;
                    B[cblock + 3][i + 4] = v7;
                } // at least, B[0 - 3] loaded
                for (j = cblock ; j < cblock + 4 ; j++) {
                    v0 = B[j][rblock + 4];
                    v1 = B[j][rblock + 5];
                    v2 = B[j][rblock + 6];
                    v3 = B[j][rblock + 7];
                    v4 = A[rblock + 4][j];
                    v5 = A[rblock + 5][j];
                    v6 = A[rblock + 6][j];
                    v7 = A[rblock + 7][j];

                    B[j][rblock + 4] = v4;
                    B[j][rblock + 5] = v5;
                    B[j][rblock + 6] = v6;
                    B[j][rblock + 7] = v7;
                    
                    i = j + 4;
                    B[i][rblock] = v0;
                    B[i][rblock + 1] = v1;
                    B[i][rblock + 2] = v2;
                    B[i][rblock + 3] = v3;
                } // at least, A[4 - 7] loaded, and B[0 - 3] is done
                for (i = rblock + 4 ; i < rblock + 8 ; i++) {
                    v0 = A[i][cblock + 4];
                    v1 = A[i][cblock + 5];
                    v2 = A[i][cblock + 6];
                    v3 = A[i][cblock + 7];

                    B[cblock + 4][i] = v0;
                    B[cblock + 5][i] = v1;
                    B[cblock + 6][i] = v2;
                    B[cblock + 7][i] = v3;
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

