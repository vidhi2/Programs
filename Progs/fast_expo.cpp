// author :iamvoid13(Lovekesh Garg)

/*
	fast exponentiation
	in O(lg n) time
*/

#include <iostream>

using namespace std;

typedef unsigned long long ull;

ull power_loop(int a,int b);
ull power_recursion(int a,int b);

int main(){
	int a,b;
	cout<<"Enter a and b :"<<endl;
	cin>>a>>b;
	cout<<"a^b :"<<endl;
	cout<<"---By Loops : "<<power_loop(a,b)<<endl;
	cout<<"---By recursion : "<<power_recursion(a,b)<<endl;
	return 0;
}
ull power_loop(int a,int b){
	ull ans=1;
	while(b){
		if(b%2!=0)
			ans*=a;
        a*=a;
		b/=2;
	}
	return ans;
}
ull power_recursion(int a,int b){
	if(b==0)
		return 1;
	if(b==1)
		return a;
	ull z=power_recursion(a,b/2),ans;
	ans=z*z;
	if(b%2!=0)
		ans*=a;
	return ans;
}
