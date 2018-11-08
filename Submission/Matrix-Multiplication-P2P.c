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
	rowA = 20;
	colA = 20;
	rowB = 20;
	colB = 20;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0) {
		double startTime = MPI_Wtime();
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
		for (i = 1; i < size; ++i) {
			MPI_Send(&partRow, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&rowA, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&rowB, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
			MPI_Send(&colA, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
			MPI_Send(&colB, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
			MPI_Send(&matrixA[(i - 1) * partRow * colA], partRow * colA, MPI_INT, i, 5, MPI_COMM_WORLD);
			MPI_Send(matrixB, rowB * colB, MPI_INT, i, 6, MPI_COMM_WORLD);
		}
		for (i = (size - 1) * partRow; i < rowA; ++i) {
			for (j = 0; j < colB; ++j) {
				int temp = 0;
				for (k = 0; k < colA; ++k) {
					temp += matrixA[i * colA + k] * matrixB[k * colB + j];
				}
				matrixC[i * colB + j] = temp;
			}
		}
		for (i = 1; i < size; ++i) {
			MPI_Recv(&matrixC[(i - 1) * partRow * colB], partRow * colB, MPI_INT, i, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		double endTime = MPI_Wtime();
		printf("Calculating Time: %fs\n", endTime - startTime);
	}
	else {
		MPI_Recv(&partRow, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&rowA, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&rowB, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&colA, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&colB, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		matrixB = (int*) malloc(rowB * colB * sizeof(int));
		matrixABuf = (int*) malloc(partRow * colA * sizeof(int));
		matrixCBuf = (int*) malloc(partRow * colB * sizeof(int));
		MPI_Recv(&matrixABuf[0], partRow * colA, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(matrixB, rowB * colB, MPI_INT, 0, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (i = 0; i < partRow; ++i) {
			for (j = 0; j < colB; ++j) {
				int temp = 0;
				for (k = 0; k < colA; ++k) {
					temp += matrixABuf[i * colA + k] * matrixB[k * colB + j];
				}
				matrixCBuf[i * colB + j] = temp;
			}
		}
		MPI_Send(matrixCBuf, partRow * colB, MPI_INT, 0, 7, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}