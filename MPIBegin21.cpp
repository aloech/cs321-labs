#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin21");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	double a;
	int n;
	pt >> a >> n;

	MPI_Send(&a, 1, MPI_DOUBLE, n, 0, MPI_COMM_WORLD);

	MPI_Recv(&a, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
	pt << a << status.MPI_SOURCE;
}
