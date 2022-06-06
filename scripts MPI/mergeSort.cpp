
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
    //low为第1有序区的第1个元素，i指向第1个元素, mid为第1有序区的最后1个元素
    int i = low, j = mid + 1, k = 0;  //mid+1为第2有序区第1个元素，j指向第1个元素
    double* temp = new double[high - low + 1]; //temp数组暂存合并的有序序列
    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) //较小的先存入temp中
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid)//若比较完之后，第一个有序区仍有剩余，则直接复制到t数组中
        temp[k++] = arr[i++];
    while (j <= high)//同上
        temp[k++] = arr[j++];
    for (i = low, k = 0; i <= high; i++, k++)//将排好序的存回arr中low到high这区间
        arr[i] = temp[k];
    delete[]temp;//释放内存，由于指向的是数组，必须用delete []
}


void mergeSort(double arr[], int low, int high) {
    if (low >= high) { return; } // 终止递归的条件，子序列长度为1
    int mid = low + (high - low) / 2;  // 取得序列中间的元素
    //mergeSort(arr, low, mid);  // 对左半边递归
    //mergeSort(arr, mid + 1, high);  // 对右半边递归

if(flag>0){
flag--;
#pragma omp parallel sections//并行区域
		{
#pragma omp section//负责这个区域的线程对前面部分进行排序
		mergeSort(arr, low, mid); 
#pragma omp section//负责这个区域的线程对后面部分进行排序
		mergeSort(arr, mid + 1, high);
		}
}
else{
    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);
}
//mergeSort(arr, low, mid);
//mergeSort(arr, mid + 1, high);

merge(arr, low, mid, high);  // 合并
return;

}


void mergeSortSerial(double arr[], int low, int high) {
    if (low >= high) { return; } // 终止递归的条件，子序列长度为1
    int mid = low + (high - low) / 2;  // 取得序列中间的元素
    mergeSort(arr, low, mid);  // 对左半边递归
    mergeSort(arr, mid + 1, high);  // 对右半边递归
    merge(arr, low, mid, high);  // 合并
}




int main() {
    initialArray();
    //travelArray();
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