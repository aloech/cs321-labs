#include <windows.h>
#pragma hdrstop
#include "pt4.h"
#include "omp.h"

const auto THREAD_COUNT = 4;

double Run(double x, int n)
{
	auto y = 0.0;

	#pragma omp parallel reduction(+:y)
	{
		auto count = 0,
			 gr_size = omp_get_num_threads() == 1 ? 1 : THREAD_COUNT,
			 k = omp_get_thread_num();
		auto start = omp_get_wtime();

		for (auto group = 0; group < n / gr_size; ++group) 
		{
			auto i = gr_size * group + (group % 2 ? gr_size - k : k + 1);
			auto z = 0.0;

			for (auto j = 1; j <= i; ++j) 
			{
				z += (j + sin(x + j)) / (2 * i * j - 1);
				count++;
			}

			y += 1 / z;
		}

		if (omp_get_num_threads() != 1)
			#pragma omp critical
			{
				Show("Thread num: ", k);
				Show("Count: ", count);
				ShowLine("Thread time: ", omp_get_wtime() - start);
			}
	}

	return y;
}

void Solve()
{
	Task("OMPBegin5");
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
