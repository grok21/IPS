#include <iostream>
#include <omp.h>

double par_4(void)
{
	long long num = 100000000;
	double step;
	int num_of_threads = 3;
	long long inc = 0;
	long long i = 0;
	double x = 0.0;
	double pi;
	double S = 0.0;
	step = 1.0 / (double)num;
	double t = omp_get_wtime();

	// Parallelize the loop into 3 threads
	#pragma omp parallel for  num_threads(num_of_threads) reduction(+:x)  reduction(+:S)
	for (i = 0; i < num; i++)
	{
		x = (i + 0.5)*step;
		S = S + 4.0 / (1.0 + x * x);

		// Replaced atomic to critical
		#pragma omp critical
		inc++;
	}
	t = omp_get_wtime() - t;
	pi = step * S;
	printf("Par: pi = %.14f\n", pi);
	return t;
}

int task4_run()
{
	printf("time: %f sec.\n\n", par_4());
	return 0;
}

