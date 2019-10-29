#include<mpi.h>
#include<stdio.h>

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
	//printf("Hello from %i/%i\n", rank, nProcesses);
	int leader = rank;
	int recvLeader;
	int primesc = 0;
	int n = 200;
	while(n) {
		for (int i  = 0; i < 30; i++) {
			if (graph[i][0] == rank) {
				MPI_Send(&leader, 1, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD);
				primesc++;
			}
		}
	
		while(primesc) {
			MPI_Recv(&recvLeader, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			if (recvLeader < leader) {
				leader = recvLeader;
			}
			primesc--;
		}	
	n--;
	}
	printf("%d %d\n",rank, leader);
	

	//printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}