/*
@author: Nguyen Viet Sang
ID: 1512798
@date: 24/10/2018
**/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void printMatrix(int *matrix, int N){
    for (int i = 0; i < N * N; i++){
        printf("%d %d ", i, matrix[i]);
    }
    printf("\n");
}

int main(int argc, char **argv){
    int N = 2000; //Matrix size
    int MASTER = 0;

    int size, rank, nWorkers, source, dest;
    int nRowEachP; //rows for each process
    int i, j, k; //counter
    //int i, rank, size;

    int *mat_a;
    int *mat_b;
    int *mat_c;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    nWorkers = size - 1;

    if (rank == MASTER){
        mat_a = (int*) malloc(N * N * sizeof(int));
        mat_b = (int*) malloc(N * N * sizeof(int));
        mat_c = (int*) malloc(N * N * sizeof(int));

        for (i = 0; i < N * N; i++){
            mat_a[i] = rand() % 100;
            mat_b[i] = rand() % 100;
        }
        //printMatrix(mat_a, N);
        //printMatrix(mat_b, N);
        nRowEachP = N / size;

        double startTime = MPI_Wtime(); 

        for (dest = 1; dest <= nWorkers; i++){
            MPI_Send(&nRowEachP, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&N, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&mat_a[(i - 1) * nRowEachP * N], nRowEachP * N, MPI_INT, dest, 2, MPI_COMM_WORLD);
            MPI_Send(&mat_b[0], N * N, MPI_INT, dest, 3, MPI_COMM_WORLD);
        }

        //Master do its job for the rest of mat_a
        for (i = (size - 1) * nRowEachP; i < N; i++){
            for (j = 0; j < N; j++){
                int temp = 0;
                for (k = 0; k < N; k++){
                    temp += mat_a[i * N + k] * mat_b[k * N + j];
                }
                mat_c[i * N + j] = temp;
            }
        }

        //Receive
        for (source = 1; source <= nWorkers; i++){
            MPI_Recv(&mat_c[(i - 1) * nRowEachP * N], nRowEachP * N, MPI_INT, source, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        //printMatrix(mat_a, N);
        //printMatrix(mat_b, N);
        //printMatrix(mat_c, N);
        //Time
        printf("Time: %f\n", MPI_Wtime() - startTime);
    }

    if (rank != MASTER){
        int *mat_b_recv = (int*) malloc(N * N * sizeof(int));
        int *mat_a_recv = (int*) malloc(nRowEachP * N * sizeof(int));
        int *mat_c_send = (int*) malloc(nRowEachP * N * sizeof(int));

        MPI_Recv(&nRowEachP, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&N, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&nRowEachP, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&mat_a_recv, nRowEachP * N, MPI_INT, MASTER, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&mat_b_recv, N * N, MPI_INT, MASTER, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (i = 0; i < nRowEachP; i++){
            for (j = 0; j < N; j++){
                int temp = 0;
                for (k = 0; k < N; k++){
                    temp += mat_a_recv[i * N + k] * mat_b_recv[k * N + j];
                }
                mat_c_send[i * N + j] = temp;
            }
        }

        MPI_Send(&mat_c_send[0], nRowEachP * N, MPI_INT, MASTER, 4, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}