#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin96");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm matrix_comm;
	MPI_Status status;

	double a;
	pt >> a;

	auto n = size / 2,
		 ndims = 2,
		 *dims = new int[ndims] { 2, n },
		 *periods = new int[ndims] { 1, 1 };
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &matrix_comm);

	int rank_source, rank_dest;
	MPI_Cart_shift(matrix_comm, 1, 1, &rank_source, &rank_dest);

	double res;
	MPI_Sendrecv(&a, 1, MPI_DOUBLE, rank_dest, 0, 
		         &res, 1, MPI_DOUBLE, rank_source, 0, matrix_comm, &status);
	pt << res;

	MPI_Comm_free(&matrix_comm);
	delete[] dims;
	delete[] periods;
}
