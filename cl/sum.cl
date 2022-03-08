__kernel void		addition(__global const int *A, __global const int *B, __global int *C)
{
    int i = get_global_id(0);    // получаем индекс обрабатываемого элемента, он-же позиция элемента в массиве
    C[i] = A[i] + B[i];          // выполняем сложение
}
