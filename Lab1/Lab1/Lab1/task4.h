#include <iostream>
#include <vector>
#include <ctime>
#include <tbb/tbb.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <mutex>

using namespace std;

mutex int_mutex; 

double func4(double x)
{
	return 4 / pow((1 + pow(x, 2)), 2);
}

double trapezoid_integral4(double left_point, double right_point, int points_amount)
{
	double length = right_point - left_point;
	double dx = length / points_amount;
	cilk::reducer_opadd<double> integral_sum(0.0);

	tbb::parallel_for(int(0), points_amount, [&](auto i)
	{
		int_mutex.lock();
		integral_sum += dx * (func4(i * dx + left_point) + func4((i + 1) * dx + left_point)) / 2;
		int_mutex.unlock();
	});

	return integral_sum.get_value();
}

void task4_run()
{
    double left_point = -1;
    double right_point = 1;

	for (int points_amount = 100; points_amount < 1000000000; points_amount *= 10)
	{
		auto start = clock() / 1000.0;
		double result = trapezoid_integral4(left_point, right_point, points_amount);;
		auto run_time = clock() / 1000.0;
		cout << "Points amount: " << points_amount << "   Result: " << result << "   Time: " << run_time - start << endl; 
	}
}