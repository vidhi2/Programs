// Lovekesh Garg
// Merge Sort

#include <iostream>

using namespace std;

void init(string s);
void MergeSort(int a[],int p,int q);
void Merge(int a[],int p,int mid,int q);

int main(){

	init("Merge Sort");

	int n,i;
	cout << "How many numbers do you wanna to sort : ";
	cin >> n;
	int a[n];
	cout << "Enter the numbers :" <<endl;
	for(i=0;i<n;i++)
		cin >> a[i] ;
	MergeSort(a,0,n-1);

	for(i=0;i<n;i++)
		cout << a[i] << " ";
	cout << endl;
	return 0;
}

void init(string s){
	cout << "\t\t\t\t" << s <<endl;
}

void MergeSort(int a[],int p,int q){
	if(p<q){
		int mid = (p+q)/2;
		
		MergeSort(a,p,mid);
		MergeSort(a,mid+1,q);

		Merge(a, p, mid, q);
	}
	return ;
}

void Merge(int a[],int p,int mid,int q){
	int b[q-p+1];
	int left = p;
	int right = mid+1;
	int k=0;
	while(k<=q-p && left <= mid && right <=q ){
		if(left <= mid && a[left] < a[right]){
			b[k] = a[left++];
		}else if(right <= q && a[right] <= a[left]){
			b[k] = a[right++];
		}
		k++;
	}
	if(left == mid+1){
		for(;k<=q-p && right <= q;k++){
			b[k] = a[right++];
		}
	}else if(right == q+1){
		for(;k<=q-p && left <= mid;k++){
			b[k] = a[left++];
		}
	}
	
	for(int i=0;i<=q-p;i++)
		a[p+i] = b[i];

	return ;
}