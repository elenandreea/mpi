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
    int buffer[nProcesses];
    int bufferRecv[nProcesses];
    int primesc = 0;
    
    for (int i = 0; i < nProcesses; i++) {
                buffer[i] = -1;
    }

    if (rank == 0) {
        

        for (int i = 0; i < 30; i++) {
            if (graph[i][0] == 0) {
                 MPI_Send(buffer, nProcesses, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD);
                 primesc++;
            }
       }

        while(primesc) {
            MPI_Recv(bufferRecv, nProcesses, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < nProcesses; i++) {
                if (buffer[i] == -1 && bufferRecv[i] != -1) {
                        buffer[i] = bufferRecv[i]; 
                }

            }
            primesc--;
        }

        for(int i = 0; i < nProcesses; i++) {
            printf("%d ", buffer[i]);
        }

    } else {
        MPI_Recv(bufferRecv, nProcesses, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        buffer[rank] = status.MPI_SOURCE;
        for (int i = 0; i < 30; i++) {
            if (graph[i][0] == rank) {
                if (graph[i][1] != status.MPI_SOURCE) {
                    MPI_Send(buffer, nProcesses, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD);
                    primesc++;
                }
            }      
        }

        while(primesc) {
            MPI_Recv(bufferRecv, nProcesses, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < nProcesses; i++) {
                if (buffer[i] == -1 && bufferRecv[i] != -1) {
                        buffer[i] = bufferRecv[i]; 
                }

            }
            primesc--;
        }

        MPI_Send(buffer, nProcesses, MPI_INT, buffer[rank], 0, MPI_COMM_WORLD);
        
    }
	

	printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}
