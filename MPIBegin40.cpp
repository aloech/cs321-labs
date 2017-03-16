#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin40");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double num;
	pt >> num;

	auto buf = new double[size];
	MPI_Allgather(&num, 1, MPI_DOUBLE, buf, 1, MPI_DOUBLE, MPI_COMM_WORLD);

	for (auto i = 0; i < size; ++i)
		pt << buf[i];
}
