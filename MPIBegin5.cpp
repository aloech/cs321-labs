#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPIBegin5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n;
	pt >> n;

	double sum = 0;

	for (auto i = 0; i < n; ++i) {
		double val;
		pt >> val;
		sum += val;
	}

	pt << (rank % 2 == 0 ? sum : sum / n);
}
