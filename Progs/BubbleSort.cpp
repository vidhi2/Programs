// Lovekesh Garg
// Bubble Sort

#include <iostream>

using namespace std;

void init(string s);
void swap(int a[],int p,int q);

int main(){

	init("Bubble Sort");

	int n,i,j;
	cout << "How many numbers do you wanna to sort : ";
	cin >> n;
	int a[n];
	cout << "Enter the numbers :" <<endl;
	for(i=0;i<n;i++)
		cin >> a[i] ;
	for(i=0;i<n-1;i++){
		for(j=n-1;j>i;j--){
			if(a[j] < a[j-1]){
				swap(a,j-1,j);
			}
		}
	}
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