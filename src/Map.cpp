#include <iostream>
#include <cstdio>
#include <map>
using namespace std;
int main(){
	map<string,int> name;
	name["Ravi"]=1;
	name["Keshav"]=2;
	name["Lovekesh"]=3;
	name["Shubham"]=4;
	name["Anjan"]=5;
	if(name.find("shreyans")==name.end()){
		printf("name not found");
	}
	return 0;
}