#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin26");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	double num;
	int tag;

	if (rank == 0)
	{
		for (auto i = 1; i < size; i++) {
			MPI_Recv(&num, 1, MPI_DOUBLE, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
			pt << num;
		}
		return;
	}

	pt >> num >> tag;
	MPI_Send(&num, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
}
