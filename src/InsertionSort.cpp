// Lovekesh Garg
// Insertion Sort

#include <iostream>

using namespace std;

void init(string s);

int main(){

	init("Insertion Sort");

	int n,i,j,key;
	cout << "How many numbers do you wanna to sort : ";
	cin >> n;
	int a[n];
	cout << "Enter the numbers :" <<endl;
	for(i=0;i<n;i++)
		cin >> a[i] ;

	for(i=1;i<n;i++){
		key = a[i];
		j = i-1;
		while(j>=0 && key < a[j]){
			a[j+1] = a[j];
			j--;
		}
		a[j+1] = key;
	}

	for(i=0;i<n;i++)
		cout << a[i] << " ";
	cout << endl;
	return 0;
}

void init(string s){
	cout << "\t\t\t\t" << s <<endl;
}
