#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"
void Solve()
{
    Task("MPIBegin72");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const auto evens_size = (size + 1) / 2;
	auto *evens = new int[evens_size];
	for (auto i = 0; i < evens_size; i++)
		evens[i] = i * 2;

	MPI_Group all, odds;
	const auto odds_size = size - evens_size;
	MPI_Comm_group(MPI_COMM_WORLD, &all);
	MPI_Group_excl(all, evens_size, evens, &odds);

	MPI_Comm comm;
	MPI_Comm_create(MPI_COMM_WORLD, odds, &comm);

	if (comm == MPI_COMM_NULL)
		return;

	const auto count = 2;
	double send_buf[count];
	for (auto i = 0; i < count; i++)
		pt >> send_buf[i];

	const auto recv_count = count * odds_size;
	auto *recv_buf = new double[recv_count];

	MPI_Allgather(send_buf, count, MPI_DOUBLE, recv_buf, count, MPI_DOUBLE, comm);

	for (auto i = 0; i < recv_count; i++)
		pt << recv_buf[i];

	MPI_Group_free(&all);
	MPI_Group_free(&odds);
	MPI_Comm_free(&comm);
	delete[] evens;
	// delete[] send_buf;
	delete[] recv_buf;
}
