#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
	int rank, size;
	int rowA, colA, rowB, colB;
	int partRow;
	int *matrixA, *matrixB, *matrixC;
	int *matrixABuf, *matrixCBuf;
	int i, j, k;
	double startTime, endTime;
	rowA = 2000;
	colA = 2000;
	rowB = 2000;
	colB = 2000;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0) {
		startTime = MPI_Wtime();
		matrixA = (int*) malloc(rowA * colA * sizeof(int));
		matrixB = (int*) malloc(rowB * colB * sizeof(int));
		matrixC = (int*) malloc(rowA * colB * sizeof(int));
		for (i = 0; i < rowA * colA; ++i) {
			matrixA[i] = rand() % 10;
		}
		for (i = 0; i < rowB * colB; ++i) {
			matrixB[i] = rand() % 10;
		}
		partRow = rowA / size;
	}
	MPI_Bcast(&partRow, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&rowA, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&rowB, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&colA, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&colB, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank != 0) {
		matrixB = (int*) malloc(rowB * colB * sizeof(int));
	}
	matrixABuf = (int*) malloc(partRow * colA * sizeof(int));
	matrixCBuf = (int*) malloc(partRow * colB * sizeof(int));
	MPI_Bcast(matrixB, rowB * colB, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(matrixA, partRow * colA, MPI_INT, matrixABuf, partRow * colA, MPI_INT, 0, MPI_COMM_WORLD);
	for (i = 0; i < partRow; ++i) {
		for (j = 0; j < colB; ++j) {
			int temp = 0;
			for (k = 0; k < colA; ++k) {
				temp += matrixABuf[i * colA + k] * matrixB[k * colB + j];
			}
			matrixCBuf[i * colB + j] = temp;
		}
	}
	MPI_Gather(matrixCBuf, partRow * colB, MPI_INT, matrixC, partRow * colB, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (i = size * partRow; i < rowA; ++i) {
			for (j = 0; j < colB; ++j) {
				int temp = 0;
				for (k = 0; k < colA; ++k) {
					temp += matrixA[i * colA + k] * matrixB[k * colB + j];
				}
				matrixC[i * colB + j] = temp;
			}
		}
		endTime = MPI_Wtime();
		printf("Calculating Time: %fs\n", endTime - startTime);
	}
	MPI_Finalize();
	return 0;
}