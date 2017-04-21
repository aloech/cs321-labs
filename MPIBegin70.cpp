#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPIBegin70");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	char *recv_buf = nullptr,
		 *send_buf = nullptr;
	int *recv_counts = nullptr,
		*displs = nullptr,
		*int_buf = nullptr,
		position = 0;
	double double_buf;

	if (rank)
	{
		int send_buf_size, int_buf_size, double_buf_size;
		MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &double_buf_size);
		MPI_Pack_size(rank, MPI_INT, MPI_COMM_WORLD, &int_buf_size);
		send_buf_size = double_buf_size + int_buf_size;
		send_buf = new char[send_buf_size];

		pt >> double_buf;

		int_buf = new int[rank];
		for (auto i = 0; i < rank; ++i)
			pt >> int_buf[i];

		MPI_Pack(&double_buf, 1, MPI_DOUBLE, 
			     send_buf, send_buf_size, &position, MPI_COMM_WORLD);
		MPI_Pack(int_buf, rank, MPI_INT, 
			     send_buf, send_buf_size, &position, MPI_COMM_WORLD);
		MPI_Send(send_buf, send_buf_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
	}

	if (!rank)
	{
		auto recv_double_count = 0;
		recv_counts = new int[size];
		displs = new int[size];

		for (auto i = 0; i < size; ++i)
		{
			recv_double_count += i;
			recv_counts[i] = !i ? 0 : i + 1;
			displs[i] = !i ? 0 : displs[i - 1] + recv_counts[i - 1];
		}

		int int_recv_size, double_recv_size;
		MPI_Pack_size(size, MPI_INT, MPI_COMM_WORLD, &int_recv_size);
		MPI_Pack_size(
			recv_double_count, MPI_DOUBLE, MPI_COMM_WORLD, &double_recv_size);

		auto recv_buf_size = int_recv_size + double_recv_size;
		recv_buf = new char[recv_buf_size];
		int_buf = new int[size];

		for (auto i = 1; i < size; ++i)
		{
			position = 0;
			MPI_Recv(recv_buf, recv_buf_size, MPI_PACKED, i, 0, 
				MPI_COMM_WORLD, &status);
			MPI_Unpack(recv_buf, recv_buf_size, &position, 
				       &double_buf, 1, MPI_DOUBLE, MPI_COMM_WORLD);
			pt << double_buf;

			MPI_Unpack(recv_buf, recv_buf_size, &position, 
				       int_buf, i, MPI_INT, MPI_COMM_WORLD);
			for (auto j = 0; j < i; ++j)
				pt << int_buf[j];
		}
	}

	delete[] recv_buf;
	delete[] recv_counts;
	delete[] displs;
	delete[] int_buf;
	delete[] send_buf;
}
