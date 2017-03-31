#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPIBegin66");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int int_buf_size, double_buf_size;
	MPI_Pack_size(size, MPI_INT, MPI_COMM_WORLD, &int_buf_size);
	MPI_Pack_size(size, MPI_DOUBLE, MPI_COMM_WORLD, &double_buf_size);
	const auto buf_size = int_buf_size + double_buf_size;

	auto buf = new char[buf_size];
	auto position = 0;
	auto int_buf = new int[size];
	auto double_buf = new double[size];

	if (!rank)
	{
		for (auto i = 0; i < size; ++i)
			pt >> int_buf[i];

		for (auto i = 0; i < size; ++i)
			pt >> double_buf[i];

		MPI_Pack(int_buf, size, MPI_INT, buf, buf_size, &position, MPI_COMM_WORLD);
		MPI_Pack(double_buf, size, MPI_DOUBLE, buf, buf_size, &position, MPI_COMM_WORLD);	
	}

	MPI_Bcast(buf, buf_size, MPI_PACKED, 0, MPI_COMM_WORLD);

	if (rank)
	{
		MPI_Unpack(buf, buf_size, &position, int_buf, size, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(buf, buf_size, &position, double_buf, size, MPI_DOUBLE, MPI_COMM_WORLD);

		for (auto i = 0; i < size; ++i)
			pt << int_buf[i];

		for (auto i = 0; i < size; ++i)
			pt << double_buf[i];
	}

	delete[] buf;
	delete[] int_buf;
	delete[] double_buf;
}
