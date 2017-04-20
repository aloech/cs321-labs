#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin94");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm ingrid, grid;

	auto ndims = 2,
		 * dims = new int[ndims];
	if (!rank) 
		pt >> dims[0] >> dims[1];
	MPI_Bcast(dims, ndims, MPI_INT, 0, MPI_COMM_WORLD);

	auto col = rank >= dims[0] * dims[1] ? MPI_UNDEFINED : 1;
	MPI_Comm_split(MPI_COMM_WORLD, col, rank, &ingrid);

	if (ingrid == MPI_COMM_NULL) 
		return;

	auto periods = new int[ndims] { 0, 1 };
	MPI_Cart_create(ingrid, ndims, dims, periods, 0, &grid);

	auto coords = new int[ndims];
	pt >> coords[0] >> coords[1];
	MPI_Cart_rank(grid, coords, &rank);

	pt << rank;

	MPI_Comm_free(&ingrid);
	MPI_Comm_free(&grid);
	delete[] dims;
	delete[] periods;
	delete[] coords;
}
