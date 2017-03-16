#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin51");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const auto nums_size = size + 5;
	auto nums = new double[nums_size];

	for (auto i = 0; i < nums_size; ++i)
		pt >> nums[i];

	auto buf = new double[nums_size];
	MPI_Allreduce(nums, buf, nums_size, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

	for (auto i = 0; i < nums_size; ++i)
		pt << buf[i];
}
