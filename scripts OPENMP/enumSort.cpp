#include<iostream>
#include<stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <omp.h>
using namespace std;



const int arrayLen = 500000;
double myArray[arrayLen] = {};


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


void enumSort(double myArray[arrayLen]) {
    double* arrayPtr = new double[arrayLen];
    bool* judgePtr = new bool[arrayLen];

    for (int i = 0; i < arrayLen; i++) {
        judgePtr[i] = false;
    }//to see if the member is empty
    


//并行区域
int i, j,count=0;

//double begin = omp_get_wtime();
//cout<<begin<<endl;
#pragma omp parallel private(i,j,count)
{
#pragma omp for
    for (i = 0; i < arrayLen; i++)
    {
        count = 0;

        for (j = 0; j < arrayLen; j++)
        {
            if (myArray[i] >= myArray[j])
            {
                count++;
            }
        }      
        arrayPtr[count - 1] = myArray[i];
        judgePtr[count - 1] = true;
    }//find trival value
}	
//double end = omp_get_wtime();
//cout<<end<<endl;
//cout << "parallel time" << (double)(end - begin)<< endl;




    double temp = 0;

    for (int k = arrayLen - 1; k >= 0; k--) {
        if (!judgePtr[k]) {
            //cout << k << "null";
            arrayPtr[k] = temp;
        }
        temp = arrayPtr[k];
    }//fill the empty member



    for (int i = 0; i < arrayLen; i++) {
        myArray[i] = arrayPtr[i];
    }//copy
    

    cout << endl;
    delete[]arrayPtr;
    delete[]judgePtr;
}


void enumSortSerial(double myArray[arrayLen]) {
    int count = 0;
    double* arrayPtr = new double[arrayLen];
    bool* judgePtr = new bool[arrayLen];
    for (int i = 0; i < arrayLen; i++) {
        judgePtr[i] = false;
    }//to see if the member is empty

//double begin = omp_get_wtime();
//cout<<begin<<endl;
    for (int i = 0; i < arrayLen; i++)
    {
        count = 0;
        for (int j = 0; j < arrayLen; j++)
        {
            if (myArray[i] >= myArray[j])
            {
                count++;
            }
        }
        arrayPtr[count-1] = myArray[i];
        judgePtr[count - 1] = true;
    }//find trival value


    double temp = 0;
    for (int k = arrayLen - 1; k >= 0; k--) {
        if (!judgePtr[k]) {
            //cout << k << "null";
            arrayPtr[k] = temp;
        }
        temp = arrayPtr[k];
    }//fill the empty member
//double end = omp_get_wtime();
//cout<<end<<endl;
//cout << "serial time" << (double)(end - begin)<< endl;




    for (int i = 0; i < arrayLen; i++) {
        myArray[i] = arrayPtr[i];
    }//copy

    cout << endl;
    delete[]arrayPtr;
    delete[]judgePtr;
}


int main() {
    initialArray();
    //travelArray();
    cout << "The size of the array is: " << arrayLen << endl;
    double begin = omp_get_wtime();
    enumSort(myArray);
    double end = omp_get_wtime();
    cout << "parallel time" << (double)(end - begin)<< endl;
    //travelArray();

    initialArray();
begin = omp_get_wtime();
    enumSortSerial(myArray);
end = omp_get_wtime();
    cout << "serial time" << (double)(end - begin)  << endl;

}