#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin4");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (!rank)
		return;

	if (!(rank % 2))
  {
		int n;
		pt >> n;
		pt << n * 2;
		return;
	}

	double n;
	pt >> n;
	pt << n * 2;
}
