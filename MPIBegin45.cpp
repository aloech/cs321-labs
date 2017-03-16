#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin45");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	const auto nums_size = size * (size + 1) / 2;
	auto nums = new int[nums_size];

	for (auto i = 0; i < nums_size; ++i)
		pt >> nums[i];

	const auto recv_count = rank + 1,
		       buf_size = recv_count * size;
	auto buf = new int[buf_size],
		 send_counts = new int[size],
		 send_displs = new int[size],
		 recv_counts = new int[size],
		 recv_displs = new int[size];

	for (auto i = 0; i < size; ++i)
	{
		send_counts[i] = i + 1;
		send_displs[i] = !i ? 0 : send_displs[i - 1] + i;
		recv_counts[i] = recv_count;
		recv_displs[i] = !i ? 0 : recv_displs[i - 1] + recv_count;
	}

	MPI_Alltoallv(nums, send_counts, send_displs, MPI_INT, 
		buf, recv_counts, recv_displs, MPI_INT, MPI_COMM_WORLD);

	for (auto i = 0; i < buf_size; ++i)
		pt << buf[i];
}
