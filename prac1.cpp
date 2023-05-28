#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <omp.h>

using namespace std;

// Function to perform BFS from a given vertex
void bfs(int startVertex, vector<bool> &visited, vector<vector<int>> &graph)
{
// Create a queue for BFS
queue<int> q;
// Mark the start vertex as visited and enqueue it
visited[startVertex] = true;
q.push(startVertex);
// Loop until the queue is empty
while (!q.empty())
{
// Dequeue a vertex from the queue
int v = q.front();
q.pop();
// Enqueue all adjacent vertices that are not visited
for (int i = 0; i < graph[v].size(); i++)
{
int u = graph[v][i];

if (!visited[u])
{
visited[u] = true;
q.push(u);
}
}
}
}

// Parallel Breadth-First Search
void parallelBFS(vector<vector<int>> &graph, int numCores)
{
int numVertices = graph.size();
vector<bool> visited(numVertices, false); // Keep track of visited vertices
double startTime = omp_get_wtime(); // Start timer

// Perform BFS from all unvisited vertices using OpenMP tasks
#pragma omp parallel num_threads(numCores)
{
#pragma omp single
{
for (int v = 0; v < numVertices; v++)
{
if (!visited[v])
{
#pragma omp task
bfs(v, visited, graph);

}
}
}
}

double endTime = omp_get_wtime(); // End timer
cout << "Number of cores used: " << numCores << endl;
cout << "Time taken: " << endTime - startTime << " seconds" << endl;
cout << " " << endl;
}

int main()
{
// Generate a random graph with 10,000 vertices and 50,000 edges
int numVertices = 10000;
int numEdges = 50000;
vector<vector<int>> graph(numVertices);
srand(time(0));
for (int i = 0; i < numEdges; i++)
{
int u = rand() % numVertices;
int v = rand() % numVertices;
graph[u].push_back(v);
graph[v].push_back(u);
}

// Array containing number of cores

int numCoresArr[] = {1, 2, 3, 4};

// Loop over different number of cores and execute parallel BFS
for (int i = 0; i < sizeof(numCoresArr) / sizeof(numCoresArr[0]); i++)
{
int numCores = numCoresArr[i];
parallelBFS(graph, numCores);
}

return 0;
}