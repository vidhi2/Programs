// Lovekesh Garg
// Heap Sort

#include <iostream>

#define PARENT(n) ( (n-1) / 2 )
#define LEFT(n) ( 2 * n + 1 )
#define RIGHT(n) ( 2 * n + 2 ) 

using namespace std;

void HeapSort(int a[],int n);
void init(string s);
void swap(int a[],int p,int q);
void MaxHeapify(int a[],int i,int n);
void BuildMaxHeap(int a[],int n);
int main(){

	init("Heap Sort");

	int n,i,j;
	cout << "How many numbers do you wanna to sort : ";
	cin >> n;
	int a[n];
	cout << "Enter the numbers :" <<endl;
	for(i=0;i<n;i++)
		cin >> a[i] ;
	HeapSort(a,n);
	for(i=0;i<n;i++)
		cout << a[i] << " ";
	cout << endl;
	return 0;
}
void HeapSort(int a[],int n){
	int i;
	int heapSize = n;
	BuildMaxHeap(a,n);
	for( i=0 ; i<n ;i++ , heapSize--){
		MaxHeapify(a,0,heapSize);
		swap(a,0,n-i-1);
	}
	return ;
}
void BuildMaxHeap(int a[],int n){
	for(int i=PARENT(n);i>=0;--i){
		MaxHeapify(a,i,n);
	}
	return ;
}
void MaxHeapify(int a[],int i,int n){
	int largest = i;
	int l = LEFT(i);
	int r = RIGHT(i);
	if( l<n && a[l]>a[i] ){
		largest = l;
	}
	if(r<n && a[r]>a[largest] ){
		largest = r;
	}
	if( largest!=i ){
		swap(a,i,largest);
		MaxHeapify(a,largest,n);
	}
	return ;
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
