
// author : iamvoid13 (Lovekesh Garg)

/*
	A simple C++ program to calculate n!.
	Both by :
		recursion and non_recursion(loops)
*/

#include <iostream>

using namespace std;

typedef unsigned long long ull;

ull fac_rec(int n);
ull fac_not_rec(int n);

int main(){
	int t,i;
	cout<<"How many numbers do you have : ";
	cin>>t;
	cout<<"Now enter "<<t<<" numbers :"<<endl;
	for(i=0;i<t;i++){
		int n;
		cout<<"--Enter your number : ";
		cin>>n;
		cout<<"The value of"<<n<<"! is "<<endl;
		cout<<" By recursion :"<<fac_rec(n)<<endl;
		cout<<" By non-recursion :"<<fac_not_rec(n)<<endl;
	}
	return 0;
}
ull fac_rec(int n){
	if(n<2)
		return 1;
	return n*fac_rec(n-1);
}
ull fac_not_rec(int n){
	ull ans=1;
	int i;
	for(i=1;i<=n;i++)
		ans=ans*i;
	return ans;
}
