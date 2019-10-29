#include<mpi.h>
#include<stdio.h>
#include <math.h>

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 12 ./a.out
 */

int graph[][2] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, 
                  { 3, 4 }, { 4, 11 }, { 11, 5 }, 
                  { 5, 6 }, { 6, 7 }, { 7, 8 },
                  { 8, 9 }, { 9, 10 }, { 10, 9 },
                  { 9, 8 }, { 8, 7 }, { 7, 6 },
                  { 6, 5 }, { 5, 11 }, { 11, 4 },
                  { 4, 3 }, { 3, 2 }, { 2, 1 },
                  { 1, 0 }, { 9, 5 }, { 5, 9 },
                  { 5, 3 }, { 3, 5 }, { 0, 2 },
                  { 2, 0 }, { 9, 7 }, { 7, 9 } };


int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	float n;
	float nRecv;
	int ciclez = 100;
	int primesc = 0;


	if (rank == 0) {
		n = 1;
	} else {
		n = 0;
	}
	
	while(ciclez) {
		for (int i  = 0; i < 30; i++) {
			if (graph[i][0] == rank) {
				MPI_Send(&n, 1, MPI_FLOAT, graph[i][1], 0, MPI_COMM_WORLD);
				MPI_Recv(&nRecv, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
				n = (n + nRecv) / 2;
			}
		}	
		ciclez--;
	}

	printf("%f \n", 1 / n);
	//printf("Hello from %i/%i\n", rank, nProcesses);

	

	//printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}