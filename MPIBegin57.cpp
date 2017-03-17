#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin57");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const auto nums_size = size + 5;
	auto nums = new int[nums_size];

	for (auto i = 0; i < nums_size; ++i)
		pt >> nums[i];

	auto buf = new int[nums_size];
	MPI_Scan(nums, buf, nums_size, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	for (auto i = 0; i < nums_size; ++i)
		pt << buf[i];

	delete[] nums;
	delete[] buf;
}
