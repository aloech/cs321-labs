#include <windows.h>
#pragma hdrstop
#include "mpi.h"
#include "pt4.h"

const auto COUNT = 3;

struct Triad
{
	int fst, snd;
	double thd;

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
    Task("MPIBegin65");
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
	auto oldtypes = new MPI_Datatype[COUNT] { MPI_INT, MPI_INT, MPI_DOUBLE };
	MPI_Type_struct(COUNT, blocklens, triad_displs, oldtypes, &MPI_TRIAD);
	MPI_Type_commit(&MPI_TRIAD);

	auto send_buf = new Triad[rank]; 
	for (auto i = 0; i < rank; ++i)
		pt >> send_buf[i];

	auto recv_buf_size = 0;
	Triad *recv_buf = nullptr;
	int *recv_counts = nullptr,
		*displs = nullptr;

	if (!rank)
	{
		for (auto i = 0; i < size; ++i)
			recv_buf_size += i;

		recv_buf = new Triad[recv_buf_size];
		recv_counts = new int[size];
		displs = new int[size];

		for (auto i = 0; i < size; ++i) 
		{
			recv_counts[i] = i;
			displs[i] = !i ? 0 : displs[i - 1] + recv_counts[i - 1]; 
		}
	}

	MPI_Gatherv(send_buf, rank, MPI_TRIAD, recv_buf, recv_counts, displs, MPI_TRIAD, 0, MPI_COMM_WORLD);

	if (!rank)
		for (auto i = 0; i < recv_buf_size; ++i) 
			pt << recv_buf[i];

	delete[] blocklens;
	delete[] triad_displs;
	delete[] oldtypes;
	delete[] send_buf;
	delete[] recv_buf;
	delete[] recv_counts;
	delete[] displs;
	MPI_Type_free(&MPI_TRIAD);
}
