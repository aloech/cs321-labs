#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPIBegin80");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n;
	pt >> n;

	MPI_Comm comm;
	MPI_Comm_split(MPI_COMM_WORLD, n, rank, &comm);

	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);

	int send_buf;
	pt >> send_buf;
	auto count = 1;
	auto *recv_buf = new int[size];

	MPI_Allgather(&send_buf, count, MPI_INT, recv_buf, count, MPI_INT, comm);

	for (auto i = 0; i < size; ++i)
		pt << recv_buf[i];

	MPI_Comm_free(&comm);
	delete[] recv_buf;
}
