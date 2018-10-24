/*
@author: Nguyen Viet Sang
ID: 1512798
@date: 24/10/2018
**/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv){
    int N = 1500;
    int MASTER = 0;
    int FROM_MASTER = 1;
    int FROM_WORKER = 2;

    int size, rank, num_workers, source, dest, n_bytes, m_types, int_size, db_size, averow, extra;
    int i, j, k;
    //int i, rank, size;

    int a[N*N];
    int b[N*N];
    int c[N*N];

    int offset[1];
    int rows[1];

    long computeTime[1];
    long maxComputeTime[1];


    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("%d %d", rank, size);
    MPI_Finalize();

    return 0;
}