#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	if (rank == 0)
	{
		int k, n, sum = 0;
		pt >> k;

		for (auto i = 0; i < k; i++) {
			MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			sum += n;
		}

		pt << sum;
		return;
	}
	
	int n;
	pt >> n;
	if (n > 0) 
		MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}
