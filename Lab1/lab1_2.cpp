#include <iostream>
#include <ctime>
#include <thread>

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

template<typename T>
T trapezoid_integral_thread(T left_point, T right_point, unsigned int points_amount)
{
    T result = 0;
    thread t1([&]() {result += trapezoid_integral<double>(left_point, right_point / 2, points_amount / 2); });
    thread t2([&]() {result += trapezoid_integral<double>(right_point / 2, right_point, points_amount / 2); });
    t1.join();
    t2.join();
    return result;
}

template<typename T>
T trapezoid_integral_qpar(T left_point, T right_point, unsigned int points_amount)
{
    T length = right_point - left_point;
    T current_point = left_point;
    T dx = length / points_amount;
    T integral_sum = 0;

    #pragma loop(hint_parallel(2))
    for (int i = 0; i < points_amount; i++)
    {
        integral_sum += dx * (func(current_point) + func(current_point + dx)) / 2;
        current_point += dx;
    }
    return integral_sum;
}

enum calculate_type
{
    sync = 1,
    threads = 2,
    qpar = 3
};

template<typename T>
T calculate_integral(calculate_type type, T left_point, T right_point, unsigned int points_amount)
{
    switch (type)
    {
    case sync:
        return trapezoid_integral<T>(left_point, right_point, points_amount);
        break;

    case threads:
        return trapezoid_integral_thread<T>(left_point, right_point, points_amount);
        break;

    case qpar: 
        return trapezoid_integral_qpar<T>(left_point, right_point, points_amount);
        break;

    default: 
        return trapezoid_integral<T>(left_point, right_point, points_amount);
        break;
    }
}

void launch_tests()
{
    cout << "========================================" << endl << endl;
    for (int points_amount = 100; points_amount < 1000000000; points_amount *= 10)
    {
        auto start = clock() / 1000.0;

        double result_sync   = calculate_integral<double>(sync,   -1, 1, points_amount);
        auto sync_time = clock() / 1000.0 - start; 
        cout << "Type: sync      " << "Result: " << result_sync << "   Points amount: " << points_amount << "   Time: " << sync_time << endl;

        double result_thread = calculate_integral<double>(threads, -1, 1, points_amount);
        auto thread_time = clock() / 1000.0 - sync_time; 
        cout << "Type: threads   " << "Result: " << result_thread << "   Points amount: " << points_amount << "   Time: " << thread_time << endl;

        double result_qpar   = calculate_integral<double>(qpar,   -1, 1, points_amount);
        auto qpar_time = clock() / 1000.0 - thread_time; 
        cout << "Type: qpar      " << "Result: " << result_qpar << "   Points amount: " << points_amount << "   Time: " << qpar_time << endl << endl;

        cout << "========================================" << endl << endl;
    }
}

int main()
{
    launch_tests();
    system("pause");
    return 0;
}