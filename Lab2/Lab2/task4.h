#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>

// Function: CompareForAndCilk_For
//   Fills vector using for,
//   fills reducer vector using cilk_for and
//   prints both durations. 
// 
// Arguments: 
//   sz - vectors' sizes  
//
void CompareForAndCilk_For(size_t sz)
{
	std::vector <int> vector;
	cilk::reducer<cilk::op_vector<int>>red_vec;
	
	auto t1 = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < sz; i++)
		vector.push_back(rand() % 20000 + 1);
	
	auto t2 = std::chrono::high_resolution_clock::now();
	cilk_for(size_t i = 0; i < sz; i++)
		red_vec->push_back(rand() % 20000 + 1);
	
	auto t3 = std::chrono::high_resolution_clock::now();
	std::cout << "Vector with for duration: " << std::chrono::duration<double>(t2 - t1).count() << "\n";
	std::cout << "Reducer with cilk_for duration: " << std::chrono::duration<double>(t3 - t2).count() << "\n";
}

// Function: task4_run
//   Calls CompareForAndCilk_For with different sizes
// 
// Arguments: -   
//
void task4_run()
{
	std::vector<size_t> sz{ 1000000, 100000, 10000, 1000, 500, 100, 50, 10 };
	std::for_each(sz.begin(), sz.end(), [](size_t sz)) 
	{
		std::cout << "Array size: " << sz << "\n";
		CompareForAndCilk_For(sz); 
		std::cout << "\n";
	});		
}