#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin36");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const auto recv_count = 3, 
		       nums_size = size * recv_count;
	auto *nums = new double[nums_size],
		 *recv_buf = new double[recv_count];
	int *send_counts = nullptr,
		*displs = nullptr;

	if (!rank)
	{
		for (auto i = 0; i < nums_size; ++i)
			pt >> nums[i];

		send_counts = new int[size];
		displs = new int[size];
		for (auto i = 0; i < size; ++i) 
		{
			send_counts[i] = recv_count;
			displs[i] = i * send_counts[i];
		}
	}

	MPI_Scatterv(nums, send_counts, displs, MPI_DOUBLE, recv_buf, 
		recv_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	for (auto i = 0; i < recv_count; ++i)
		pt << recv_buf[i];
}
