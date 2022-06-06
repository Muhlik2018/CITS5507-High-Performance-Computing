
#include<iostream>
#include<stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <omp.h>
using namespace std;



const int arrayLen = 500000;
double myArray[arrayLen] = {};
int flag=1;


void initialArray() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < arrayLen; i++) {
        myArray[i] = (rand() % (10));
    }
}

void travelArray() {
    for (int i = 0; i < arrayLen; i++) {
        cout << &myArray[i] << " ";
        cout << myArray[i] << endl;
    }
}

void merge(double arr[], int low, int mid, int high) {

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


void mergeSort(double arr[], int low, int high) {
    if (low >= high) { return; } 
    int mid = low + (high - low) / 2;  


if(flag>0){
flag--;
#pragma omp parallel sections
		{
#pragma omp section
		mergeSort(arr, low, mid); 
#pragma omp section
		mergeSort(arr, mid + 1, high);
		}
}
else{
    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);
}


merge(arr, low, mid, high); 
return;

}


void mergeSortSerial(double arr[], int low, int high) {
    if (low >= high) { return; } 
    int mid = low + (high - low) / 2;  
    mergeSort(arr, low, mid);  
    mergeSort(arr, mid + 1, high);  
    merge(arr, low, mid, high); 
}




int main() {
    initialArray();
    cout << "The size of the array is: " << arrayLen << endl;
double begin = omp_get_wtime();
    mergeSort(myArray, 0, arrayLen - 1);
double end = omp_get_wtime();
    cout << "parallel time" << (double)(end - begin)<<endl;
    //travelArray();

    initialArray();
begin = omp_get_wtime();
    mergeSortSerial(myArray, 0,arrayLen-1);
end = omp_get_wtime();
    cout << "serial time" << (double)(end - begin) << endl;

}