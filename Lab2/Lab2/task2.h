#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>
#include <cilk/reducer_vector.h>
#include <chrono>
#include <iostream>

// Function: task2_ReducerMaxTest
//   Finds max element and in the passed array
// 
// Arguments: 
//   mass_pointer - array pointer
//   size - array's size
//
void task2_ReducerMaxTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_max_index<long, int>> maximum;
	cilk_for(long i = 0; i < size; ++i)
		maximum->calc_max(i, mass_pointer[i]);
	
	std::cout << "Max = " << maximum->get_reference() 
		<< ", index = " << maximum->get_index_reference() << "\n";
}

// Function: task2_ReducerMinTest
//   Finds max element and in the passed array
// 
// Arguments: 
//   mass_pointer - array pointer
//   size - array's size
//
void task2_ReducerMinTest(int *mass_pointer, const long size)
{
	cilk::reducer<cilk::op_min_index<long, int>> minimum;
	cilk_for(long i = 0; i < size; ++i)
		minimum->calc_min(i, mass_pointer[i]);

	std::cout << "Min = " << minimum->get_reference() 
		<< ", index = " << minimum->get_index_reference() << "\n";
}

// Function: task2_ParallelSort
//   Sorts passed array in ascending order
// 
// Arguments: 
//   begin - pointer at array's first element 
//   end - pointer at array's last element
//
void task2_ParallelSort(int *begin, int *end)
{		
	if (begin != end)
	{
		--end;
		int *middle = std::partition(begin, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *middle);
		cilk_spawn task2_ParallelSort(begin, middle);
		task2_ParallelSort(++middle, ++end);
		cilk_sync;
	}		
}

// Function: task2_run
//   Creates array of selected size, fills with random elements, 
//   finds min and max elements using task2_ReducerMinTest and task2_ReducerMaxTest,
//   sorts it using task2_ParallelSort and finds needed elements again. 
//   All results will be printed to console.
// 
// Arguments: - 
//
void task2_run()
{
	srand((unsigned)time(0));

	__cilkrts_set_param("nworkers", "4");
	const long mass_size = 10000;
	int *mass_begin, *mass_end;
	int *mass = new int[mass_size];

	for (int i = 0; i < mass_size; ++i)
		mass[i] = (rand() % 25000) + 1;
		
	mass_begin = mass;
	mass_end = mass_begin + mass_size;
	task2_ReducerMinTest(mass, mass_size);
	task2_ReducerMaxTest(mass, mass_size);
	
	task2_ParallelSort(mass_begin, mass_end);
	task2_ReducerMinTest(mass, mass_size);
	task2_ReducerMaxTest(mass, mass_size);
	
	delete[]mass;	
}