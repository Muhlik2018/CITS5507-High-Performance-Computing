#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include<iostream>
using namespace std;

double serial_start_time, serial_end_time, end_time, start_time;



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

void enumSortSerial(double* myArray, int arrayLen) {
	int count = 0;
	double* arrayPtr = new double[arrayLen];
	bool* judgePtr = new bool[arrayLen];
	for (int i = 0; i < arrayLen; i++) {
		judgePtr[i] = false;
	}//to see if the member is empty


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
		arrayPtr[count - 1] = myArray[i];
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


	for (int i = 0; i < arrayLen; i++) {
		myArray[i] = arrayPtr[i];
	}//copy


	//cout << "serial sorting complete, result is: " << endl;

	//travelArray(arrayLen, myArray);
	delete[]arrayPtr;
	delete[]judgePtr;
}



int ErrMsg(char* msg){
	printf("Error: %s \n", msg);
	return 1;
}


int CountRank(double* data, int DataSize, int MyLength, int* rank, int SumID, int MyID){
	int i, j;
	int start, end;

	start = DataSize / SumID * MyID;    
	end = start + MyLength;             

	for (j = start; j < end; j++) {       
		rank[j - start] = 0;
		for (i = 0; i < DataSize; i++) {
			if ((data[j] > data[i]) || ((data[j] == data[i]) && (j > i)))
				rank[j - start]++;
		}
	}
	return 1;
}





int main(int argc,char *argv[])
{
	int DataSize, MyLength;            
	double *data_in, *data_out, *data_serial;            
	int *rank;                       
	int MyID, SumID;
	int i, j;         

	MPI_File fh;
	MPI_Status status;                   
 
	MPI_Init(&argc,&argv);                
	MPI_Comm_rank(MPI_COMM_WORLD,&MyID);  
        MPI_Comm_size(MPI_COMM_WORLD,&SumID); 
 
	if(MyID==0)                          
		DataSize = 500000;
	
	MPI_Bcast(&DataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_File_open(MPI_COMM_WORLD, "test.txt", MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
                                             
	MyLength=DataSize/SumID;              
	if(MyID==SumID-1)                    
		MyLength=DataSize-MyLength*(SumID-1);
 
	data_in=(double *)malloc(DataSize*sizeof(double)); 
	if(data_in==0) ErrMsg("Malloc memory error!");
 
 
	if(MyID==0){ 

		data_out=(double *)malloc(DataSize*sizeof(double)); 
		if(data_out==0) ErrMsg("Malloc memory error!");

		data_serial = (double*)malloc(DataSize * sizeof(double)); 
		if (data_serial == 0) ErrMsg("Malloc memory error!");
 
		rank=(int *)malloc(DataSize*sizeof(int));     
		if(rank==0) ErrMsg("Malloc memory error!");
	}
	else{
		rank=(int *)malloc(MyLength*sizeof(int));    
		if(rank==0) ErrMsg("Malloc memory error!");
	}
 
	if(MyID==0){
		MPI_File_read(fh, data_in, DataSize, MPI_DOUBLE, MPI_STATUS_IGNORE);
		copyArray(data_in, data_serial, DataSize);
		//cout << "array readed is: "<<endl;
		//travelArray(DataSize, data_in);
		//cout << endl;
		start_time = MPI_Wtime();
	}
 
	MPI_Bcast(data_in, DataSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	CountRank(data_in,DataSize,MyLength,rank,SumID,MyID);
 
	if(MyID==0){
		for(i=1;i<SumID;i++){
			if(i==SumID-1)
				MPI_Recv(rank+MyLength*i,DataSize-MyLength*(SumID-1),MPI_INT,i,0,MPI_COMM_WORLD,&status);
			else
				MPI_Recv(rank+MyLength*i,MyLength,MPI_INT,i,0,MPI_COMM_WORLD,&status);
		}
	}
	else
		MPI_Send(rank,MyLength,MPI_INT,0,0,MPI_COMM_WORLD);
 
	if(MyID==0){
		for(i=0;i<DataSize;i++)
			data_out[rank[i]]=data_in[i];
		end_time = MPI_Wtime();
		//cout << "paraSorting compelte, result is: " << endl;
		//travelArray(DataSize, data_out);
		serial_start_time = MPI_Wtime();
		enumSortSerial(data_serial, DataSize);
		serial_end_time = MPI_Wtime();
		cout << "Array size is: " << DataSize << endl;
		cout << "serialSorting time comsumption is: " << serial_end_time - serial_start_time << endl;
		cout << "paraSorting time comsumption is: " << end_time - start_time << endl;
		cout << "speedup ratio is: " << (serial_end_time - serial_start_time) / (end_time - start_time) << endl;
		//MPI_File_write(fh, data_in, DataSize, MPI_INT, MPI_STATUS_IGNORE);

		printf("\n");
	}





	MPI_File_close(&fh);
	MPI_Finalize();   
        return 1;
}
