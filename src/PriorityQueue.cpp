// Lovekesh Garg
// Priority Queue

#include <iostream>

#define MAX 1000
#define PARENT(n) ( (n-1) / 2 )
#define LEFT(n) ( 2 * n + 1 )
#define RIGHT(n) ( 2 * n + 2 ) 

using namespace std;

void init(string s);
void swap(int a[],int p,int q);
void MaxHeapify(int a[],int i,int n);
void BuildMaxHeap(int a[],int n);


class PriorityQueue
{
public:
	PriorityQueue();
	void Insert(int val);
	int getMax();
	int extractMax();
	void increaseKey(int index,int newVal);
	int getSize();
	void printList();
private:
	int size;
	int a[MAX];
};

int main(){

	init("Priority Queue");

	PriorityQueue myQueue;
	myQueue.printList();
	myQueue.extractMax();
	for (int i = 0; i < 10; ++i){
		myQueue.Insert(i);
	}
	myQueue.printList();
	cout << myQueue.getMax() << endl;
	cout << myQueue.extractMax() << endl;
	myQueue.printList();
	myQueue.Insert(15);
	myQueue.printList();
	myQueue.increaseKey(5,36);
	myQueue.printList();
	return 0;
}

// class PriorityQueue functions
PriorityQueue::PriorityQueue(){
	size = 0;
}
void PriorityQueue::Insert(int val){
	a[size] = val;
	size++;
	BuildMaxHeap(a,size);
	return ;
}
int PriorityQueue::getMax(){
	if(size == 0)
		cerr << "List is Empty!!" << endl;
	return a[0];
}
int PriorityQueue::extractMax(){
	if(size == 0)
		cerr << "Invalid Operation" << endl;
	if(size >0){
		swap(a, 0 , size-1 );
		size--;
		MaxHeapify(a,0,size);
		return a[size];
	}
}
void PriorityQueue::increaseKey(int index,int newVal){
	int i=index-1;
	a[i] = newVal;
	while(i){
		MaxHeapify(a,PARENT(i),size);
		i=PARENT(i);
	}
	return ;
}
int PriorityQueue::getSize(){
	return size;
}
void PriorityQueue::printList(){
	if(size == 0){
		cerr << "List is Empty!!" << endl;
		return ;
	}
	for (int i = 0; i < size; ++i){
		cout << a[i] << " ";
	}
	cout << endl;
	return ;
}

// helper functions
void init(string s){
	cout << "\t\t\t\t" << s <<endl;
}
void swap(int a[],int p,int q){
	int z=a[p];
	a[p]=a[q];
	a[q]=z;
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
void BuildMaxHeap(int a[],int n){
	for(int i=PARENT(n);i>=0;--i){
		MaxHeapify(a,i,n);
	}
	return ;
}