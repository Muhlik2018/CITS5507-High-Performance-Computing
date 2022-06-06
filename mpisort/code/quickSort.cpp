
#include"mpi.h"
#include<time.h>
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
#define TRUE 1
#define SIZE 500000


double wht_start_time, wht_end_time, end_time, start_time;


void copyArray(double* ori, double* dest, int arrayLen) {
    for (int i = 0; i < arrayLen; i++) {
        dest[i] = ori[i];
    }
}


void travelArray(int length, double* tempArray) {
    for (int i = 0; i < length; i++) {
        cout << &tempArray[i] << " ";
        cout << tempArray[i] << endl;
    }
}


int Partition(double* wht_data, int wht_start, int wht_end)
{
    int wht_pivo;
    int wht_i, wht_j;
    int wht_tmp;
    wht_pivo = wht_data[wht_end];
    wht_i = wht_start - 1; 
    for (wht_j = wht_start; wht_j < wht_end; wht_j++)
        if (wht_data[wht_j] <= wht_pivo)
        {
            wht_i++; 
            wht_tmp = wht_data[wht_i];
            wht_data[wht_i] = wht_data[wht_j];
            wht_data[wht_j] = wht_tmp;
        }
    wht_tmp = wht_data[wht_i + 1];
    wht_data[wht_i + 1] = wht_data[wht_end];
    wht_data[wht_end] = wht_tmp;
    return wht_i + 1;
}

void QuickSort(double* wht_data, int wht_start, int wht_end)
{
    int wht_r;
    int wht_i;
    if (wht_start < wht_end)
    {
        wht_r = Partition(wht_data, wht_start, wht_end);
        QuickSort(wht_data, wht_start, wht_r - 1);
        QuickSort(wht_data, wht_r + 1, wht_end);
    }
}


int exp2(int wht_num)
{
    int wht_i;
    wht_i = 1;
    while (wht_num > 0)
    {
        wht_num--;
        wht_i = wht_i * 2;
    }
    return wht_i;
}


int log2(int wht_num)
{
    int wht_i, wht_j;
    wht_i = 1;
    wht_j = 2;
    while (wht_j < wht_num)
    {
        wht_j = wht_j * 2;
        wht_i++;
    }
    if (wht_j > wht_num)
        wht_i--;
    return wht_i;
}



void para_QuickSort(double* wht_data, int wht_start, int wht_end, int wht_m, int id, int MyID)
{
    int wht_i, wht_j;
    int wht_r;
    int MyLength;
    double* wht_tmp;
    MPI_Status status;
    MyLength = -1;

    if (wht_m == 0)
    {
        wht_start_time = MPI_Wtime();
        if (MyID == id)
            QuickSort(wht_data, wht_start, wht_end);
        return;
    }
    wht_start_time = MPI_Wtime();

    if (MyID == id)
    {

        wht_r = Partition(wht_data, wht_start, wht_end);
        MyLength = wht_end - wht_r;

        MPI_Send(&MyLength, 1, MPI_INT, id + exp2(wht_m - 1), MyID, MPI_COMM_WORLD);

        if (MyLength != 0)
            MPI_Send(wht_data + wht_r + 1, MyLength, MPI_DOUBLE, id + exp2(wht_m - 1), MyID, MPI_COMM_WORLD);
    }

    if (MyID == id + exp2(wht_m - 1))
    {
        MPI_Recv(&MyLength, 1, MPI_INT, id, id, MPI_COMM_WORLD, &status);
        if (MyLength != 0)
        {
            wht_tmp = (double*)malloc(MyLength * sizeof(double));
            if (wht_tmp == 0) printf("Malloc memory error!");
            MPI_Recv(wht_tmp, MyLength, MPI_DOUBLE, id, id, MPI_COMM_WORLD, &status);
        }
    }

    wht_j = wht_r - 1 - wht_start;
    MPI_Bcast(&wht_j, 1, MPI_INT, id, MPI_COMM_WORLD);

    if (wht_j > 0)
        para_QuickSort(wht_data, wht_start, wht_r - 1, wht_m - 1, id, MyID);

    wht_j = MyLength;
    MPI_Bcast(&wht_j, 1, MPI_INT, id, MPI_COMM_WORLD);

    if (wht_j > 0)
        para_QuickSort(wht_tmp, 0, MyLength - 1, wht_m - 1, id + exp2(wht_m - 1), MyID);

    if ((MyID == id + exp2(wht_m - 1)) && (MyLength != 0))
        MPI_Send(wht_tmp, MyLength, MPI_DOUBLE, id, id + exp2(wht_m - 1), MPI_COMM_WORLD);
    if ((MyID == id) && (MyLength != 0))
        MPI_Recv(wht_data + wht_r + 1, MyLength, MPI_DOUBLE, id + exp2(wht_m - 1), id + exp2(wht_m - 1), MPI_COMM_WORLD, &status);
}


int partition(double* array, int p, int r) {
    double x = array[r];     
    int i = p - 1;
    for (int j = p; j < r; ++j) {
        if (array[j] < x) {
            i++;
            swap(array[i], array[j]);
        }
    }
    swap(array[r], array[i + 1]);
    return i + 1;
}
void quicksort(double* array, int p, int r) {
    if (p < r) {
        int q = partition(array, p, r);
        quicksort(array, p, q - 1);
        quicksort(array, q + 1, r);
    }
}


int main(int argc, char* argv[])
{

    double wht_dataSize;
    double* wht_data;
    double* wht_data1;

    int MyID, SumID;
    int wht_i, wht_j;
    int wht_m, wht_r;
    MPI_File fh;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &MyID);

    MPI_Comm_size(MPI_COMM_WORLD, &SumID);


    MPI_File_open(MPI_COMM_WORLD, "test.txt", MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);

    if (MyID == 0)
    {

        wht_dataSize = SIZE;
        wht_data = (double*)malloc(wht_dataSize * sizeof(double));
        wht_data1 = (double*)malloc(wht_dataSize * sizeof(double));



        MPI_File_read(fh, wht_data, wht_dataSize, MPI_DOUBLE, MPI_STATUS_IGNORE);

        //cout << "array before sorting: " << endl;
        //travelArray(wht_dataSize, wht_data);
        copyArray(wht_data, wht_data1, wht_dataSize);
    }
    wht_m = log2(SumID);

    MPI_Bcast(&wht_dataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    para_QuickSort(wht_data, 0, wht_dataSize - 1, wht_m, 0, MyID);
    wht_end_time = MPI_Wtime();


    if (MyID == 0)
    {
        //cout << "array after sorting: " << endl;
        //travelArray(wht_dataSize, wht_data);

        start_time = MPI_Wtime();
        quicksort(wht_data1, 0, SIZE - 1);
        end_time = MPI_Wtime();
        cout << "Array size is: " << SIZE << endl;
        cout << "serialSorting time is: "<< end_time - start_time<<endl;
        cout << "paraSorting time is:" << wht_end_time - wht_start_time << endl;
        cout << "speedup ratio is: "<< (end_time - start_time) / (wht_end_time - wht_start_time)<<endl;
    }

    MPI_Finalize(); 
}
