#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin30");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int num;

	if (!rank)
		pt >> num;

	MPI_Bcast(&num, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank)
		pt << num;
}
