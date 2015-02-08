// Lovekesh Garg
// Quick Sort

#include <iostream>
#include <cstdlib>

using namespace std;

void init(string s);
void swap(int a[],int p,int q);
void QuickSort(int a[],int start,int end);
int Partition(int a[],int start,int end);

int main(){

	init("Quick Sort");

	int n,i,j;
	cout << "How many numbers do you wanna to sort : ";
	cin >> n;
	int a[n];
	cout << "Enter the numbers :" <<endl;
	for(i=0;i<n;i++)
		cin >> a[i];
	QuickSort(a,0,n);

	for(i=0;i<n;i++)
		cout << a[i] << " ";
	cout << endl;

	return 0;
}
void init(string s){
	cout << "\t\t\t\t" << s <<endl;
}
void swap(int a[],int p,int q){
	int z=a[p];
	a[p]=a[q];
	a[q]=z;
	return ;
}
void QuickSort(int a[],int start,int end){
	if(start +1 >= end){
		return ;
	}
	int par = Partition(a,start,end);

	QuickSort(a,start,par);
	QuickSort(a,par+1,end);

	return ;
}

int Partition(int a[],int start,int end){
	int i,j;
	int pivot_index = start +(rand() % (end-start));
	int pivot = a[pivot_index];
	i = start-1;
	for(j=start;j<end;j++){
		if(a[j]>=pivot)
			continue;
		i++;
		swap(a,j,i);
	}
	swap(a,i+1,pivot_index);
	return i+1;
}