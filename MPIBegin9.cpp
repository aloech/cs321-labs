#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin9");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	const auto count = 4;
	int nums[count];

	if (rank == 0) {
		for (auto i = 1; i < size; i++) {
			MPI_Recv(&nums, count, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

			for (auto j = 0; j < count; j++)
				pt << nums[j];
		}
		return;
	}

	for (auto i = 0; i < count; i++)
		pt >> nums[i];

	MPI_Send(&nums, count, MPI_INT, 0, 0, MPI_COMM_WORLD);
}
