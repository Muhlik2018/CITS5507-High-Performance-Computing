#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include<fstream>
#include <iostream>
#define MAX_SIZE 500000

using namespace std;
double serial_start_time, serial_end_time, end_time, start_time;

void travelArray(int length, double* tempArray) {
    for (int i = 0; i < length; i++) {
        cout << &tempArray[i] << " ";
        cout << tempArray[i] << endl;
    }
}

void copyArray(double* ori, double* dest, int arrayLen) {
    for (int i = 0; i < arrayLen; i++) {
        dest[i] = ori[i];
    }
}



void merge(double *arr, int low, int mid, int high) {
    int i = low, j = mid + 1, k = 0;  
    double* temp = new double[high - low + 1]; 
    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) 
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= high)
        temp[k++] = arr[j++];
    for (i = low, k = 0; i <= high; i++, k++)
        arr[i] = temp[k];
    delete[]temp;
}


void mergeSortSerial(double *arr, int low, int high) {
    if (low >= high) { return; }
    int mid = low + (high - low) / 2; 
    mergeSortSerial(arr, low, mid); 
    mergeSortSerial(arr, mid + 1, high); 
    merge(arr, low, mid, high);  
}


int main(int argc, char* argv[])
{
    double* arr;
    double* arrForSerial;
    int comm_sz;
    int my_rank;
    int length = MAX_SIZE;
    int size;
    double* sub;
    int* tmp;
    double* result = NULL;
    int i;
    MPI_File fh;


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_File_open(MPI_COMM_WORLD, "test.txt", MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);

    if (my_rank == 0) {
        arr = (double*)malloc(length * sizeof(double));
        arrForSerial = (double*)malloc(length * sizeof(double));
        result = (double*)malloc(length * sizeof(double)); 
        MPI_File_read(fh, arr, MAX_SIZE, MPI_DOUBLE, MPI_STATUS_IGNORE);
        copyArray(arr, arrForSerial, MAX_SIZE);
        //travelArray(MAX_SIZE, arr);
        start_time = MPI_Wtime();

    }


    size = length / comm_sz; 

    sub = (double*)malloc(size * sizeof(double)); 




    MPI_Scatter(arr, size, MPI_DOUBLE, sub, size, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

    mergeSortSerial(sub, 0, size - 1);

    MPI_Gather(sub, size, MPI_DOUBLE, result, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);  



    if (my_rank == 0) {
        //mergeSortSerial(result, 0, length - 1);
        int count = 1;
        while (count< comm_sz) {
            merge(result, 0, size * count - 1, size * (count + 1) - 1);
            count++;   
        }
        if (size * comm_sz < length) {
            merge(result, 0, size * comm_sz - 1, length - 1);
        }


        end_time = MPI_Wtime();
        cout << "Array size is: " << MAX_SIZE<<endl;
        //travelArray(MAX_SIZE, result);
        serial_start_time = MPI_Wtime();
        mergeSortSerial(arrForSerial, 0, length - 1);
        serial_end_time = MPI_Wtime();
        cout << "serialSorting time comsumption is: " << serial_end_time - serial_start_time << endl;
        cout << "paraSorting time comsumption is: " << end_time - start_time << endl;
        cout << "speedup ratio is: " << (serial_end_time - serial_start_time) / (end_time - start_time) << endl;
    }

    MPI_Finalize();
    return 0;
}