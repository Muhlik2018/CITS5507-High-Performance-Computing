#include <iostream>
#include<stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <omp.h>
using namespace std;

const int arrayLen = 500000;
const int coreNumber = 4;
double myArray[arrayLen] = {};
double* startPtr = &myArray[0];
double* endPtr = &myArray[arrayLen - 1];
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

void swap(double* p1, double* p2) {
	double* temp = new double;
	*temp = *p1;
	*p1 = *p2;
	*p2 = *temp;
	delete temp;
}

void quickSort(double* startPtr, double* endPtr) {//parallel solution
	if (startPtr < endPtr) {
		double* tempStart = startPtr;
		double* tempEnd = endPtr;
		while (tempEnd != tempStart) {
			while ((tempEnd > tempStart) && (*tempEnd >= *startPtr)) {
				tempEnd--;
			}
			while ((tempEnd > tempStart) && (*tempStart <= *startPtr)) {
				tempStart++;
			}
			swap(tempStart, tempEnd);
		}
		swap(tempStart, startPtr);
if(flag>0){
flag--;//falag to controll threads number, avoid too much threads is created during the itreation
#pragma omp parallel sections//并行区域
		{
#pragma omp section//负责这个区域的线程对前面部分进行排序
		quickSort(startPtr, tempStart-1);
#pragma omp section//负责这个区域的线程对后面部分进行排序
		quickSort(tempStart+1, endPtr);
		}
}
else{
	quickSort(startPtr, tempStart-1);
	quickSort(tempStart+1, endPtr);
}
	}

	return;
}


void quickSortSerial(double* startPtr, double* endPtr) {//serial solution
	if (startPtr < endPtr) {
		double* tempStart = startPtr;
		double* tempEnd = endPtr;
		while (tempEnd != tempStart) {
			while ((tempEnd > tempStart) && (*tempEnd >= *startPtr)) {
				tempEnd--;
			}
			while ((tempEnd > tempStart) && (*tempStart <= *startPtr)) {
				tempStart++;
			}
			swap(tempStart, tempEnd);
		}
		swap(tempStart, startPtr);
		quickSortSerial(startPtr, tempStart - 1);
		quickSortSerial(tempStart + 1, endPtr);
	}
	return;
}





int main()
{
	initialArray();
	//travelArray();
	cout<<"The size of the array is: "<<arrayLen<<endl;
double begin = omp_get_wtime();
	quickSort(startPtr, endPtr);
double end = omp_get_wtime();
	cout<<"parallel time"<<(double)(end - begin)<<endl;
	//travelArray();//travel to see if the array is sorted

	//inital for serial solution
	initialArray();
begin = omp_get_wtime();
	quickSortSerial(startPtr, endPtr);
end = omp_get_wtime();
	cout<<"serial time"<<(double)(end - begin)<<endl;
	//travelArray();//travel to see if the array is sorted

	return 0;
}