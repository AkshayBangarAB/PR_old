#include <omp.h>
#include <stdlib.h>
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

using namespace std;
void s_bubble(int *, int);
void p_bubble(int *, int);
void swap(int &, int &);
void s_bubble(int *a, int n)
{
for (int i = 0; i < n; i++)
{
int first = i % 2;
for (int j = first; j < n - 1; j += 2)
{
if (a[j] > a[j + 1])
{
swap(a[j], a[j + 1]);
}
}
}
}
void p_bubble(int *a, int n)
{
for (int i = 0; i < n; i++)
{
int first = i % 2;
#pragma omp parallel for shared(a, first) num_threads(16)
for (int j = first; j < n - 1; j += 2)
{
if (a[j] > a[j + 1])

{
swap(a[j], a[j + 1]);
}
}
}
}
void swap(int &a, int &b)
{
int test;
test = a;
a = b;
b = test;
}
int bench_traverse(std::function<void()> traverse_fn)
{
auto start = high_resolution_clock::now();
traverse_fn();
auto stop = high_resolution_clock::now();
// Subtract stop and start timepoints and cast it to required unit.
// Predefined units are nanoseconds, microseconds, milliseconds, seconds,
// minutes, hours. Use duration_cast() function.
auto duration = duration_cast<milliseconds>(stop - start);
// To get the value of duration use the count() member function on the
// duration object
return duration.count();
}
int main(int argc, const char **argv)

{
if (argc < 2)
{
cout << "Specify array length.\n";
return 1;
}
int *a, n;
n = stoi(argv[1]);
a = new int[n];
for (int i = 0; i < n; i++)
{
a[i] = rand() % n;
}
int *b = new int[n];
copy(a, a + n, b);
cout << "Generated random array of length " << n << "\n\n";
int sequentialTime = bench_traverse([&]
{ s_bubble(a, n); });
omp_set_num_threads(16);
int parallelTime = bench_traverse([&]
{ s_bubble(a, n); });
float speedUp = (float)sequentialTime / parallelTime;
float efficiency = speedUp / 16;
cout
<< "Sequential Bubble sort: " << sequentialTime << "ms\n";
cout << "Parallel (16) Bubble sort: " << parallelTime << "ms\n";
cout << "Speed Up: " << speedUp << "\n";

cout << "Efficiency: " << efficiency << "\n";
return 0;
}