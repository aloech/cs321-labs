#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin83");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm comm;

	int n;
	if (!rank)
		pt >> n; 

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	const auto ndims = 2;
	auto dims = new int[ndims] { n, size / n },
		 periods = new int[ndims] { 0, 0 };

	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &comm);
	auto coords = new int[ndims];
	MPI_Cart_coords(comm, rank, ndims, coords);	pt << coords[0] << coords[1];

	MPI_Comm_free(&comm); 
	delete[] dims;
	delete[] periods;
	delete[] coords;
}
