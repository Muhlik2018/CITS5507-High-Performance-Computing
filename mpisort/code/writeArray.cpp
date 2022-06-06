#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<stdlib.h>
#include <time.h> 
#include <stdio.h>
#include<fstream>
#include <omp.h>
#include <mpi.h>
using namespace std;

const int arrayLen = 10;
const int coreNumber = 4;
double myArray[arrayLen] = {};
double myArray2[arrayLen];
double* myArray2Ptr = myArray2;
double* startPtr = &myArray[0];
double* endPtr = &myArray[arrayLen - 1];
int flag = 1;


double tempArray[arrayLen] = {};




void initialArray() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < arrayLen; i++) {
		myArray[i] = (rand() % (10));
	}
}


void travelArray(int length=arrayLen, double* tempArray=startPtr) {
	for (int i = 0; i < length; i++) {
		cout << &tempArray[i] << " ";
		cout << tempArray[i] << endl;
	}
}


void checkArray(double* start1, double* start2, int length=arrayLen) {
	for (int i = 0; i < length; i++) {
		if (start1[i] != start2[i]) {
			cout << "incorrect!";
			return;
		}
	}
	cout << "correct";
}


void writeArray() {
	cout << "writing now";
	ofstream fout("test.txt", ios_base::binary);
	fout.write((char*)myArray, sizeof(myArray));
	fout.close();

}



void readArray(int length=arrayLen, double* array=startPtr) {
	cout << "reading now";
	ifstream fin("test.txt", ios_base::in | ios_base::binary);
	double temp;
	length;
	if (fin.is_open()) {
		for (int i = 0; i < length; i++) {
			fin.read((char*)&temp, sizeof(double));
			array[i] = temp;
		}

	}
	fin.close();
}




void mpiWrite() {
	int rank, num_of_process;
	MPI_File fh;
	MPI_Status status;
	MPI_Init(0,0);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_process);
	MPI_File_open(MPI_COMM_WORLD, "test.txt",
		MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
	if(rank==0){
        cout<<"using mpi now"<<endl;
        initialArray();
        travelArray();
		MPI_File_write(fh, startPtr, arrayLen, MPI_DOUBLE, MPI_STATUS_IGNORE);
        readArray(arrayLen,myArray2Ptr);
        travelArray(arrayLen,myArray2Ptr);
        checkArray(startPtr, myArray2Ptr);
    }
	MPI_File_close(&fh);
	MPI_Finalize();

}




void mpiRead() {
	int rank, num_of_process;
    double* arrayPtr;
    arrayPtr=(double *)malloc(arrayLen*sizeof(double));
	MPI_File fh; //Declaring a File Pointer
	MPI_Status status;
	MPI_Init(0, 0);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_process);
    MPI_File_open(MPI_COMM_WORLD, "test.txt",
		MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    if(rank==0){
    cout<<"using mpi for read"<<endl;
    MPI_File_read(fh, arrayPtr, arrayLen, MPI_DOUBLE, MPI_STATUS_IGNORE);
    travelArray(arrayLen,arrayPtr);
    }

	MPI_File_close(&fh); //Closing a File
	MPI_Finalize();

}




int main()
{
	//initialArray();
	//travelArray();

	//writeArray();
	readArray(arrayLen,myArray2Ptr);

	travelArray(arrayLen,myArray2Ptr);
	//checkArray(startPtr, myArray2Ptr);

    //mpiWrite();
    //mpiRead();




	return 0;
}