#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin43");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	auto nums = new double[size];
	for (auto i = 0; i < size; ++i)
		pt >> nums[i];

	auto buf = new double[size];
	MPI_Alltoall(nums, 1, MPI_DOUBLE, buf, 1, MPI_DOUBLE, MPI_COMM_WORLD);

	for (auto i = 0; i < size; ++i)
		pt << buf[i];

	delete[] nums;
	delete[] buf;
}
