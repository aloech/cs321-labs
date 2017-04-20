#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin92");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm comm, table_comm;

	const auto ndims = 3;
	auto dims = new int[ndims] { 2, 2, size / 4 },
		periods = new int[ndims] { 0, 0, 0 };
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &table_comm);

	auto varying_coords = new int[ndims] { 1, 1, 0 }; 
	MPI_Cart_sub(table_comm, varying_coords, &comm);
	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &rank);

	double a, res;
	pt >> a;

	MPI_Allreduce(&a, &res, 1, MPI_DOUBLE, MPI_SUM, comm);

	pt << res;

	MPI_Comm_free(&comm);
	MPI_Comm_free(&table_comm);
	delete[] dims;
	delete[] periods;
	delete[] varying_coords;
}
