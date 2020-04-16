#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <chrono>
#include <iostream>

const size_t start_size = 10000;
int *mass = new int[start_size];

// Function: task3_ReducerMaxTest
//   Finds max element and in the passed array
// 
// Arguments: 
//   mass_pointer - array pointer
//   size - array's size
//
void task3_ReducerMaxTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_max_index<long, int>> maximum;
	cilk_for(long i = 0; i < size; ++i)
		maximum->calc_max(i, mass_pointer[i]);
	
	std::cout << "Max = " << maximum->get_reference()
		<< ", index = " << maximum->get_index_reference() << "\n";
}

// Function: task3_ReducerMinTest
//   Finds max element and in the passed array
// 
// Arguments: 
//   mass_pointer - array pointer
//   size - array's size
//
void task3_ReducerMinTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_min_index<long, int>> minimum;
	cilk_for(long i = 0; i < size; ++i)
		minimum->calc_min(i, mass_pointer[i]);

	std::cout << "Min = " << minimum->get_reference()
		<< ", index = " << minimum->get_index_reference() << "\n";
}

// Function: task3_ParallelSort
//   Sorts passed array in ascending order
// 
// Arguments: 
//   begin - pointer at array's first element 
//   end - pointer at array's last element
//
void task3_ParallelSort(int *begin, int *end)
{
	if (begin != end)
	{
		--end;
		int *middle = std::partition(begin, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *middle);
		cilk_spawn task3_ParallelSort(begin, middle);
		task3_ParallelSort(++middle, ++end);
		cilk_sync;
	}
}

// Function: Resize
//   Changes size of previously created array
//   and fills it with random elements.
// 
// Arguments: 
//   sz - new size 
//
size_t Resize(int sz) 
{
	size_t newSize = start_size * sz;
	int *newArray = new int[newSize];
	for (size_t i = 0; i < newSize; i++)
		newArray[i] = (rand() % 25000) + 1;

	delete[] mass;
	mass = newArray;
	return newSize;
}

// Function: task3_run
//   Resizes previously created array 3 times, 
//   sorts it, finds min and max elements and 
//   print results to console. 
// 
// Arguments: - 
//
void task3_run()
{

	srand((unsigned)time(0));
	__cilkrts_set_param("nworkers", "4");
	int size_mult[3] = { 10,50,100 };		
	int *mass_begin, *mass_end;
	for (size_t i = 0; i < 3; i++)
	{
		std::cout << "Array size: " << start_size*size_mult[i] << "\n";
		Resize(size_mult[i]);
		mass_begin = mass;
		mass_end = mass_begin + start_size * size_mult[i];

		std::cout << "Before sorting\n";
		task3_ReducerMinTest(mass, start_size * size_mult[i]);
		task3_ReducerMaxTest(mass, start_size * size_mult[i]);

		auto t1 = std::chrono::high_resolution_clock::now();
		task3_ParallelSort(mass_begin, mass_end);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = (t2 - t1);
		
		std::cout << "After sorting\n";
		task3_ReducerMinTest(mass, start_size * size_mult[i]);
		task3_ReducerMaxTest(mass, start_size * size_mult[i]);

		std::cout << "Sorting duration: " << duration.count() << "\n\n";
	}	
		
	delete[]mass;
}