#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	const auto nums_capacity = 9; 
	double nums[nums_capacity];

	if (rank == 0)
	{
		int n;
		pt >> n;

		for (auto i = 0; i < n; i++)
			pt >> nums[i];

		for (auto i = 1; i < size - 1; i++) 
			MPI_Send(&nums[i - 1], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);

		MPI_Send(&nums[size - 2], n - size + 2, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
		return;
	}

	if (rank > 0 && rank < size - 1) {
		double num;
		MPI_Recv(&num, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		pt << num;
		return;
	}

	// rank == size - 1

	MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
	int count;
	MPI_Get_count(&status, MPI_DOUBLE, &count);
	MPI_Recv(&nums, count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

	for (auto i = 0; i < count; i++)
		pt << nums[i];
}
