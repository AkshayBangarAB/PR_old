#include <iostream>
#include <algorithm>
#include <omp.h>
#include <limits.h>

using namespace std;

int main()
{
    const int n = 10000000;
    const int chunk_size = 1000;
    const int num_cores = omp_get_max_threads();

    int *arr = new int[n];

    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100;

    double start_time = omp_get_wtime();

    int min_val = INT_MAX;
    int max_val = INT_MIN;
    int sum = 0;

#pragma omp parallel for reduction(min : min_val) reduction(max : max_val) reduction(+: sum)
for (int i = 0; i < n; i += chunk_size)
    {
        int chunk_min = INT_MAX;
        int chunk_max = INT_MIN;
        int chunk_sum = 0;

        for (int j = i; j < i + chunk_size && j < n; j++)
        {
            chunk_min = min(chunk_min, arr[j]);
            chunk_max = max(chunk_max, arr[j]);
            chunk_sum += arr[j];
        }

#pragma omp critical
        {
            min_val = min(min_val, chunk_min);
            max_val = max(max_val, chunk_max);
            sum += chunk_sum;
        }
    }

    double end_time = omp_get_wtime();

    cout << "Min value: " << min_val << endl;
    cout << "Max value: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << (double)sum / n << endl;
    cout << "Parallel time with " << num_cores << " core(s): " << (end_time - start_time) << "seconds" << endl;

    // Sequential code for comparison
    start_time = omp_get_wtime();

    int seq_min = INT_MAX;
    int seq_max = INT_MIN;
    int seq_sum = 0;

    for (int i = 0; i < n; i++)
    {
        seq_min = min(seq_min, arr[i]);
        seq_max = max(seq_max, arr[i]);
        seq_sum += arr[i];
    }

    end_time = omp_get_wtime();

    cout << "Sequential time: " << end_time - start_time << " seconds" << endl;

    delete[] arr;

    return 0;
}