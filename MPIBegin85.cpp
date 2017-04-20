#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin85");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm comm, table_comm;

	const auto ndims = 2;
	auto dims = new int[ndims] { 2, size / 2 },
		periods = new int[ndims] { 0, 0 };
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &table_comm);

	auto varying_coords = new int[ndims] { 0, 1 }; // rows
	MPI_Cart_sub(table_comm, varying_coords, &comm);
	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &rank);

	double a;
	if (!rank)
		pt >> a;

	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, comm);
	pt << a;

	MPI_Comm_free(&comm);
	MPI_Comm_free(&table_comm);
	delete[] dims;
	delete[] periods;
	delete[] varying_coords;
}
