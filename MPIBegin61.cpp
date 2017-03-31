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
    Task("MPIBegin61");
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

	const auto buf_size = size - 1;
	auto *buf = new Triad[buf_size];

	if (!rank)
		for (auto i = 0; i < buf_size; i++)
			pt >> buf[i];

	MPI_Bcast(buf, buf_size, MPI_TRIAD, 0, MPI_COMM_WORLD);

	if (rank)
		for (auto i = 0; i < buf_size; i++)
			pt << buf[i];

	delete[] blocklens;
	delete[] triad_displs;
	delete[] oldtypes;
	delete[] buf;
	MPI_Type_free(&MPI_TRIAD);
}
