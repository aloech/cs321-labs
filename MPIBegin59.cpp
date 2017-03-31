#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"

const auto COUNT = 3;

struct Triad
{
	int fst, snd, thd;

	Triad() : fst(0), snd(0), thd(0) {}

	friend PTIO& operator << (PTIO& p, Triad const &t)
	{
		return p << t.fst << t.snd << t.thd;
	}

	friend PTIO& operator >> (PTIO& p, Triad &t)
	{
		return p >> t.fst >> t.snd >> t.thd;
	}
};

void Solve()
{
    Task("MPIBegin59");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Datatype MPI_TRIAD;
	auto blocklens = new int[COUNT] { 1, 1, 1 };
	auto triad_displs = new MPI_Aint[COUNT]
	{
		offsetof(Triad, fst),
		offsetof(Triad, snd),
		offsetof(Triad, thd)
	};
	auto oldtypes = new MPI_Datatype[COUNT] { MPI_INT, MPI_INT, MPI_INT };
	MPI_Type_struct(COUNT, blocklens, triad_displs, oldtypes, &MPI_TRIAD);
	MPI_Type_commit(&MPI_TRIAD);

	const auto recv_count = 1;
	auto recv_buf = new Triad[recv_count];
	int *send_counts = nullptr,
		*displs = nullptr;

	const auto send_buf_size = size - 1;
	auto send_buf = new Triad[send_buf_size];

	if (!rank)
	{
		for (auto i = 0; i < send_buf_size; ++i)
			pt >> send_buf[i];

		send_counts = new int[size];
		displs = new int[size];
		for (auto i = 0; i < size; ++i)
		{
			send_counts[i] = !i ? 0 : recv_count;
			displs[i] = i == 0 || i == 1 ? 0 : displs[i - 1] + send_counts[i];
		}
	}

	MPI_Scatterv(send_buf, send_counts, displs, MPI_TRIAD, recv_buf,
		recv_count, MPI_TRIAD, 0, MPI_COMM_WORLD);

	if (rank)
		pt << recv_buf[0];

	delete[] blocklens;
	delete[] triad_displs;
	delete[] oldtypes;
	delete[] recv_buf;
	delete[] send_counts;
	delete[] displs;
	delete[] send_buf;
	MPI_Type_free(&MPI_TRIAD);
}
