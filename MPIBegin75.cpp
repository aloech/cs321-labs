#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPIBegin75");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	auto is_even = !(rank % 2);

	MPI_Comm comm;
	MPI_Comm_split(MPI_COMM_WORLD, is_even, rank, &comm);
	MPI_Comm_rank(comm, &rank);

	double send_buf;
	pt >> send_buf;

  auto count = 1;
	double recv_buf;
	MPI_Reduce(&send_buf, &recv_buf, count, MPI_DOUBLE,
       is_even ? MPI_MAX : MPI_MIN, 0, comm);

	if (!rank)
		pt << recv_buf;

	MPI_Comm_free(&comm);
}
