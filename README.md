# MPI-threads-lab
Versions of a program that realize some operations in a matrix. Using MPI and threads in different ways and doing a benchmark.
`#define matrix_size 16000`

## Approaches 
`duo_thread.c` - single node with 2 threads sharing the task.
`quad_thread.c` - single node with 4 threads sharing the task.

`MPI_2nodes.c` - 2 single threaded nodes doing the task. 
`MPI_4nodes.c` - 4 single threaded nodes doing the task.

`MPI_Thread.c` - 2 nodes with 2 threads sharing the task in each node.

## Benchmarking
