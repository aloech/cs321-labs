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
	int a, b;

	pt >> a >> b;

	MPI_Send(&a, 1, MPI_INT, prev, 0, MPI_COMM_WORLD);
	MPI_Send(&b, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

	MPI_Recv(&a, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
	MPI_Recv(&b, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &status);

	pt << a << b;
}
