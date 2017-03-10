#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin24");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	const auto nums_capacity = 4;
	double nums[nums_capacity];

	int n;
	pt >> n;
	for (auto i = 0; i < n; i++)
		pt >> nums[i];

	auto dest = rank % 2 == 0 ? rank + 1 : rank - 1;
	int recv_n;
	MPI_Sendrecv(&n, 1, MPI_INT, dest, 0, 
		&recv_n, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &status);
	MPI_Sendrecv(&nums, n, MPI_DOUBLE, dest, 0, 
		&nums, recv_n, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD, &status);

	for (auto i = 0; i < recv_n; i++)
		pt << nums[i];
}
