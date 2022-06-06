#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<stdlib.h>
#include <time.h> 
#include <stdio.h>
#include<fstream>
#include <omp.h>
using namespace std;

const int arrayLen = 10;
const int coreNumber = 4;
double myArray[arrayLen] = {};
double* startPtr = &myArray[0];
double* endPtr = &myArray[arrayLen - 1];
int flag = 1;




void initialArray() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < arrayLen; i++) {
		myArray[i] = (rand() % (10));
	}
}

void writeArray() {
		//FILE* pFile;
		//if ((pFile = fopen("test.txt", "wb")) == NULL)
		//{
			//printf("cant open the file");
			//exit(0);
		//}
		//fwrite(myArray, sizeof(double), arrayLen, pFile);
		//fclose(pFile);

		ofstream fout("test.txt",ios_base::binary);
		fout.write((char*)myArray, sizeof(myArray));

}


void travelArray() {
	for (int i = 0; i < arrayLen; i++) {
		cout << &myArray[i] << " ";
		cout << myArray[i] << endl;
	}
}

void readArray() {
	//FILE* pFile;
	//if ((pFile = fopen("test.txt", "wb")) == NULL)
	//{
		//printf("cant open the file");
		//exit(0);
	//}
	//fread(myArray, sizeof(double), arrayLen, pFile);
	//fclose(pFile);
	ifstream fin("test.txt", ios_base::in | ios_base::binary);
	double temp;
	if (fin.is_open()) {
		for (int i = 5; i <= 10; i++) {
			fin.read((char*)&temp, sizeof(double));
			cout << temp;
		}
		cout << endl;
		while (fin.read((char*)&temp,sizeof(double)))
		{
			cout << temp;
		}

	}

}




int main()
{
	initialArray();
	writeArray();
	travelArray();//travel to see if the array is sorted
	readArray();
	//travelArray();

	return 0;
}