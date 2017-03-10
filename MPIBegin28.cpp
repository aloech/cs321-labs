#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
	Task("MPIBegin28");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	int a_int;
	double a_double;

	if (!rank) {
		int t;

		for (auto i = 1; i < size; ++i)
		{
			pt >> t;

			if (!t) 
			{
				pt >> a_int;
				MPI_Send(&a_int, 1, MPI_INT, i, t, MPI_COMM_WORLD);
				continue;
			}
			
			pt >> a_double;
			MPI_Send(&a_double, 1, MPI_DOUBLE, i, t, MPI_COMM_WORLD);
		}
		return;
	}

	MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	if (!status.MPI_TAG)
	{
		MPI_Recv(&a_int, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		pt << a_int;
		return;
	}

	MPI_Recv(&a_double, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	pt << a_double;
}
