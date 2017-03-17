#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin54");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const auto nums_size = size * 2;
	auto nums = new double[nums_size];

	for (auto i = 0; i < nums_size; ++i)
		pt >> nums[i];

	const auto recv_count = 2;
	auto recv_counts = new int[size] {2};
	auto buf = new double[recv_count];

	for (auto i = 0; i < size; ++i)
		recv_counts[i] = recv_count;

	MPI_Reduce_scatter(nums, buf, recv_counts, 
		MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
	
	for (auto i = 0; i < recv_count; ++i)
		pt << buf[i];

	delete[] nums;
	delete[] recv_counts;
	delete[] buf;
}
