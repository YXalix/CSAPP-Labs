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


void transpose_block32(int M, int N, int A[N][M], int B[M][N],int x,int y,int space){
    int a1,a2,a3,a4,a5,a6,a7,a8;
    for (int i = 0;i<space;i++){
        a1 = A[x+i][y];a2 = A[x+i][y+1];a3 = A[x+i][y+2];a4 = A[x+i][y+3];
        a5 = A[x+i][y+4];a6 = A[x+i][y+5];a7 = A[x+i][y+6];a8 = A[x+i][y+7];
        B[y+0][x+i] = a1;B[y+1][x+i] = a2;B[y+2][x+i] = a3;B[y+3][x+i] = a4;
        B[y+4][x+i] = a5;B[y+5][x+i] = a6;B[y+6][x+i] = a7;B[y+7][x+i] = a8;
    }
}

void transpose_block64(int M, int N, int A[N][M], int B[M][N],int x,int y,int space){
    int a1,a2,a3,a4,a5,a6,a7,a8;
    for (int i = x;i< x + 4;i++){
        a1 = A[ i][y];a2 = A[ i][y+1];a3 = A[ i][y+2];a4 = A[ i][y+3];
        a5 = A[ i][y+4];a6 = A[ i][y+5];a7 = A[ i][y+6];a8 = A[ i][y+7];
        B[y][i] = a1;B[y+1][i] = a2;B[y+2][i] = a3;B[y+3][i] = a4;
        B[y][i+4] = a5;B[y+1][i+4] = a6;B[y+2][i+4] = a7;B[y+3][i+4] = a8;
    }

    for (int i = y;i<y+4;i++){
        a1 = B[i][x+4];a2 = B[i][x+5];a3 = B[i][x+6];a4 = B[i][x+7];
        a5 = A[x+4][i]; a6 = A[x+5][i]; a7 = A[x+6][i]; a8 = A[x+7][i];

        B[i][x+4] = a5;B[i][x+5] = a6;B[i][x+6] = a7;B[i][x+7] = a8;

        B[i+4][x] = a1;B[i+4][x+1] = a2;B[i+4][x+2] = a3;B[i+4][x+3] = a4;
    }

    for (int i = x + 4;i< x+8;i++){
        a5 = A[ i][y+4];a6 = A[ i][y+5];a7 = A[ i][y+6];a8 = A[ i][y+7];

        B[y+4][i] = a5;B[y+5][i] = a6;B[y+6][i] = a7;B[y+7][i] = a8;
    }
}

void transpose_block61(int n,int m, int M, int N, int A[n][m], int B[m][n],int x,int y,int space){
    int a1,a2,a3,a4,a5,a6,a7,a8;
    for (int i = x;i< x + 4;i++){
        a1 = A[ i][y];a2 = A[ i][y+1];a3 = A[ i][y+2];a4 = A[ i][y+3];
        a5 = A[ i][y+4];a6 = A[ i][y+5];a7 = A[ i][y+6];a8 = A[ i][y+7];
        B[y][i] = a1;B[y+1][i] = a2;B[y+2][i] = a3;B[y+3][i] = a4;
        B[y][i+4] = a5;B[y+1][i+4] = a6;B[y+2][i+4] = a7;B[y+3][i+4] = a8;
    }

    for (int i = y;i<y+4;i++){
        a1 = B[i][x+4];a2 = B[i][x+5];a3 = B[i][x+6];a4 = B[i][x+7];
        a5 = A[x+4][i]; a6 = A[x+5][i]; a7 = A[x+6][i]; a8 = A[x+7][i];

        B[i][x+4] = a5;B[i][x+5] = a6;B[i][x+6] = a7;B[i][x+7] = a8;

        B[i+4][x] = a1;B[i+4][x+1] = a2;B[i+4][x+2] = a3;B[i+4][x+3] = a4;
    }

    for (int i = x + 4;i< x+8;i++){
        a5 = A[ i][y+4];a6 = A[ i][y+5];a7 = A[ i][y+6];a8 = A[ i][y+7];

        B[y+4][i] = a5;B[y+5][i] = a6;B[y+6][i] = a7;B[y+7][i] = a8;
    }
}

void check(int M, int N, int A[N][M], int B[M][N]){
    for(int i = 0;i<M;i++){
        for(int j = 0;j<N;j++){
            if (A[j][i] != B[i][j]){
                printf("(%d %d)\n",i,j);
            }
        }
    }
}

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
    if (M == 32){
        int space = 8;
        for (int i = 0;i<N;i+=space){
            for (int j = 0;j<M;j+=space)
                transpose_block32(M,N,A,B,i,j,space);
        }
    } else if (M == 64){
        int space = 8;
        for (int i = 0;i<N;i+=space){
            for (int j = 0;j<M;j+=space)
                transpose_block64(M,N,A,B,i,j,space);
        }
    } else {
        // 61 * 67
        int space = 8;
        int n = N / 8 * 8;
        int m = M / 8 * 8;
        int a1,a2,a3,a4,a5,a6,a7,a8;
        for (int i = 0;i<m;i+=space){
            for (int j = 0;j<n;j++){
                a1 = A[j][i];a2 = A[j][i+1];a3 = A[j][i+2];a4 = A[j][i+3];
                a5 = A[j][i+4];a6 = A[j][i+5];a7 = A[j][i+6];a8 = A[j][i+7];
                B[i+0][j] = a1;B[i+1][j] = a2;B[i+2][j] = a3;B[i+3][j] = a4;
                B[i+4][j] = a5;B[i+5][j] = a6;B[i+6][j] = a7;B[i+7][j] = a8;
            }
        }

        for (int i = 0;i<N;i++){
            for (int j = m;j<M;j++){
                B[j][i] = A[i][j];
            }
        }

        for (int i = n;i<N;i++){
            for (int j = 0;j<m;j++){
                B[j][i] = A[i][j];
            }
        }
        // check(M,N,A,B);
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
 * trans - A simple baseline transpose function for 8*8 block.
 */

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
