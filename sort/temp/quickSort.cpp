#include <iostream>
#include<stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <omp.h>
using namespace std;

const int arrayLen = 50;
double myArray[arrayLen] = {};
double* startPtr = &myArray[0];
double* endPtr = &myArray[arrayLen - 1];


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
	quickSortSerial(startPtr, endPtr);
	travelArray();



	return 0;
}