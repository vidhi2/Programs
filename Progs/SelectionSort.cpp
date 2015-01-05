// Lovekesh Garg
// Selection Sort

#include <iostream>

using namespace std;

void init(string s);
void swap(int a[],int p,int q);

int main(){

	init("Selection Sort");

	int n,i,j,min,in;
	cout << "How many numbers do you wanna to sort : ";
	cin >> n;
	int a[n];
	cout << "Enter the numbers :" <<endl;
	for(i=0;i<n;i++)
		cin >> a[i] ;

	for(i=0;i<n-1;i++){
		min = a[i];
		in = i;
		for(j=i+1;j<n;j++){
			if(a[j] < min){
				min = a[j];
				in = j;
			}
		}
		swap(a,i,in);
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