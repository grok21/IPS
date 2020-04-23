#include <iostream>
#include <ctime>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>


// ���������� ����� � �������� ���������� �������
const int MATRIX_SIZE_task4 = 1500;

/// ������� InitMatrix() ��������� ���������� � �������� 
/// ��������� ���������� ������� ���������� ����������
/// matrix - �������� ������� ����
void task4_InitMatrix(double** matrix)
{
	for (int i = 0; i < MATRIX_SIZE_task4; ++i)
		matrix[i] = new double[MATRIX_SIZE_task4 + 1];

	for (int i = 0; i < MATRIX_SIZE_task4; ++i)
		for (int j = 0; j <= MATRIX_SIZE_task4; ++j)
			matrix[i][j] = rand() % 2500 + 1;
}

/// ������� SerialGaussMethod() ������ ���� ������� ������ 
/// matrix - �������� ������� �������������� ���������, �������� � ����,
/// ��������� ������� ������� - �������� ������ ������ ���������
/// rows - ���������� ����� � �������� �������
/// result - ������ ������� ����
double task4_SerialGaussMethod(double **matrix, const int rows, double* result)
{
	int k;
	double koef;
	// ������ ��� ������ ������
	auto start = clock() / 1000.0;
	for (k = 0; k < rows; ++k)
	{
		for (int i = k + 1; i < rows; ++i)
		{
			koef = -matrix[i][k] / matrix[k][k];
			for (int j = k; j <= rows; ++j)
				matrix[i][j] += koef * matrix[k][j];
		}
	}
	auto end = clock() / 1000.0;
	auto duration = end - start;
	// �������� ��� ������ ������
	result[rows - 1] = matrix[rows - 1][rows] / matrix[rows - 1][rows - 1];
	for (k = rows - 2; k >= 0; --k)
	{
		result[k] = matrix[k][rows];
		for (int j = k + 1; j < rows; ++j)
			result[k] -= matrix[k][j] * result[j];
		result[k] /= matrix[k][k];
	}
	return duration;
}

double task4_CilkForGaussMethod(double **matrix, const int rows, double* result)
{
	int k;		
	// ������ ��� ������ ������
	auto start = clock() / 1000.0;
	for (k = 0; k < rows; ++k)
	{
		cilk_for (int i = k + 1; i < rows; ++i)
		{
			double koef = -matrix[i][k] / matrix[k][k];
			for(int j = k; j <= rows; ++j)
				matrix[i][j] += koef * matrix[k][j];
		}
	}
	auto end = clock() / 1000.0;
	auto duration = end - start;
	// �������� ��� ������ ������
	result[rows - 1] = matrix[rows - 1][rows] / matrix[rows - 1][rows - 1];
	for (k = rows - 2; k >= 0; --k)
	{
		cilk::reducer_opadd<double> result_k(matrix[k][rows]);
		cilk_for(int j = k + 1; j < rows; ++j)
			result_k -= matrix[k][j] * result[j];
		result[k] = result_k.get_value()/matrix[k][k];
	}
	return duration;
}

void task4_run()
{
	srand((unsigned)time(0));

	double **test_matrix = new double*[MATRIX_SIZE_task4];

	// ������ ������� ����
	double *result = new double[MATRIX_SIZE_task4];

	// ������������� �������
	task4_InitMatrix(test_matrix);

	auto duration = task4_CilkForGaussMethod(test_matrix, MATRIX_SIZE_task4, result);

	for (auto i = 0; i < MATRIX_SIZE_task4; ++i)
		delete[]test_matrix[i];
	

	std::cout << "-------------------------------------------------------------------------\nTask_4\n\nSolution:\n";
	for (auto i = 0; i < MATRIX_SIZE_task4; ++i)
		std::cout << "x(" << i << ") = " << result[i] << std::endl;
	
	delete[] result;
	std::cout << duration << " seconds\n\n";
}


