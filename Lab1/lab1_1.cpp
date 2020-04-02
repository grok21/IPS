#include <iostream>
#include <ctime>

using namespace std;

template<typename T>
T func(T x)
{
    return 4 / pow((1 + pow(x,2)), 2);
}

template<typename T>
T trapezoid_integral(T left_point, T right_point, unsigned int points_amount)
{
    T length = right_point - left_point;
    T current_point = left_point;
    T dx = length / points_amount;
    T integral_sum = 0;

    for (int i = 0; i < points_amount; i++)
    {
        integral_sum += dx * (func(current_point) + func(current_point + dx)) / 2;
        current_point += dx;
    }
    return integral_sum;
}

void launch_tests()
{
    for (int points_amount = 100; points_amount < 1000000000; points_amount *= 10)
    {
        auto start = clock() / 1000.0;
        double result = trapezoid_integral<double>(-1, 1, points_amount);
        auto run_time = clock() / 1000.0;
        cout << "Points amount: " << points_amount << "   Result: " << result << "   Time: " << run_time - start << endl; 
    }
}

int main()
{
    launch_tests();
    system("pause");
    return 0;
}
