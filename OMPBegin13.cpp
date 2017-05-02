#include <windows.h>
#pragma hdrstop
#include "pt4.h"
#include "omp.h"

const auto THREAD_COUNT = 4;

void Section(double x, int n, int from, int to, double &y, int &count) 
{
	for (auto i = from; i <= to; ++i) 
	{
		auto z = 0.0;

		for (auto j = 1; j <= i; ++j) 
		{
			z += (j + pow(x + j, 0.25)) / (2 * i * j - 1);
			++count;
		}

		y += 1 / z;
	}
}

double Run(double x, int n) 
{
	auto y = 0.0;

	auto k = n * (n + 1) / 2, c = 0,
		 k1 = 0, k2 = 0, k3 = 0, i = 1;

	for (auto p = 0; p < 4 - 1; ++p)
		for (; i <= n; ++i) 
		{
			c += i;

			if (c > k / 4) 
			{
				k1 = k2; 
				k2 = k3; 
				k3 = i; 
				c = 0; 
				break;
			}
		}

	auto z = 0;
	#pragma omp parallel
	{
		auto count = 0;
		auto start = omp_get_wtime();

		#pragma omp sections reduction(+:y)
		{
			#pragma omp section
			{
				Section(x, n, 1, k1, y, count);
			}

			#pragma omp section
			{ 
				Section(x, n, k1 + 1, k2, y, count);
			}

			#pragma omp section
			{ 
				Section(x, n, k2 + 1, k3, y, count); 
			}

			#pragma omp section
			{
				Section(x, n, k3 + 1, n, y, count); 
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
    Task("OMPBegin13");
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
