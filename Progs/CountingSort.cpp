// Lovekesh Garg
// Counting Sort

#include <iostream>

using namespace std;

void init(string s);
void swap(int a[],int p,int q);

int main(){

	init("Counting Sort");

	int n,i,j;
	int max=0;
	cout << "How many numbers do you wanna to sort : ";
	cin >> n;
	int a[n];
	int ans[n];
	cout << "Enter the numbers :" <<endl;
	for(i=0;i<n;i++){
		cin >> a[i];	
		if(a[i]>max)
			max=a[i];
	}
	int k=max+1;
	int freq[k];

	for(i=0;i<k;i++)
		freq[i]=0;
	for(i=0;i<n;i++)
		freq[ a[i] ]++;
	for(i=1;i<k;i++)
		freq[i]=freq[i]+freq[i-1];
	for(j=n-1;j>=0;j--){
		ans[ freq[ a[j] ] ]=a[j];
		freq[ a[j] ] --;
	}

	for(i=0;i<n;i++)
		cout << ans[i] << " ";
	cout << endl;
	return 0;
}
void init(string s){
	cout << "\t\t\t\t" << s <<endl;
}