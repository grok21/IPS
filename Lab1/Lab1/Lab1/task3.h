#include <iostream>
#include <vector>
#include <ctime>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

using namespace std;

double func3(double x)
{
	return 4 / pow((1 + pow(x, 2)), 2);
}


double trapezoid_integral3(double left_point, double right_point, int points_amount)
{
	cilk::reducer_opadd<double> integral_sum(0.0);
	double length = right_point - left_point;
	double dx = length / points_amount;

	cilk_for(int i = 0; i < points_amount; i++)
	{
		integral_sum += dx * (func3(left_point+i*dx) + func3(left_point + (i+1)*dx)) / 2;
	}

	return integral_sum.get_value();
}

void task3_run()
{
    double left_point = -1;
	double right_point = 1;
	
	for (int points_amount = 100; points_amount < 1000000000; points_amount *= 10)
	{
		auto start = clock() / 1000.0;
		auto result = trapezoid_integral3(left_point, right_point, points_amount);
		auto run_time = clock() / 1000.0;
		cout << "Points amount: " << points_amount << "   Result: " << result << "   Time: " << run_time - start << endl; 
	}
}