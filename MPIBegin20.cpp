#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	const auto prev = (rank - 1 + size) % size;
	const auto next = (rank + 1 + size) % size;
	int prev_in, prev_out, next_in, next_out;

	pt >> prev_out >> next_out;

	MPI_Send(&prev_out, 1, MPI_INT, prev, 0, MPI_COMM_WORLD);
	MPI_Send(&next_out, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

	MPI_Recv(&prev_in, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
	MPI_Recv(&next_in, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &status);

	pt << prev_in << next_in;
}
