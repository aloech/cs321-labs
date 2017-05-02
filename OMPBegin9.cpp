#include <windows.h>
#pragma hdrstop
#include "pt4.h"
#include "omp.h"

const auto THREAD_COUNT = 2;

void Section(double x, int from, int to, double &y, int &count) 
{
	for (auto i = from; i <= to; ++i) 
	{
		auto z = 0.0;

		for (auto j = 1; j <= i; ++j) 
		{
			z += (j + log(1 + x + j)) / (2 * i * j - 1);
			++count;
		}

		y += 1 / z;
	}
}

double Run(double x, int n) 
{
	auto y = 0.0;
	auto z = 0;
	#pragma omp parallel
	{
		auto count = 0;
		auto k = sqrt(n * n * 5 / 2 + n + 0.5) - n;
		auto start = omp_get_wtime();

		#pragma omp sections reduction(+:y)
		{
			#pragma omp section
			{
				Section(x, 1, k, y, count);
			}

			#pragma omp section
			{ 
				Section(x, k + 1, n, y, count); 
			}
		}

		if (omp_get_num_threads() != 1)
			#pragma omp critical
			{
				Show("Thread num: ", omp_get_thread_num());
				Show("Count: ", count);
				ShowLine("Thread time: ", omp_get_wtime() - start);
			}
	}

	return y;
}

void Solve()
{
    Task("OMPBegin9");
	double time, nptime = 0.0, ptime = 0.0,
		x, y;
	int n;

	// single-threaded
	pt >> x >> n;
	omp_set_num_threads(1);
	time = omp_get_wtime();
	y = Run(x, n);
	nptime = abs(omp_get_wtime() - time);
	ShowLine("Non-parallel time: ", nptime);
	pt << y;

	ShowLine("num_procs: ", omp_get_num_procs());
	ShowLine("num_threads: ", THREAD_COUNT);

	// multi-threaded
	pt >> x >> n;
	omp_set_num_threads(THREAD_COUNT);
	time = omp_get_wtime();
	y = Run(x, n);
	ptime = abs(omp_get_wtime() - time);
	pt << y;

	ShowLine("Total parallel time: ", ptime);
	ShowLine("Rate: ", nptime / ptime);
}
