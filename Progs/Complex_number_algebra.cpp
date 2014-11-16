
//author : iamvoid13(Lovekesh Garg)

/*
	C++ Program to illustrate COMPLEX NUMBER algebra
	by using simple operations
*/

#include <iostream>
#include <cmath>
using namespace std;

class Complex
{
public:
	//constructor overloading
	Complex(double p,double q):a(p),b(q){}
	Complex(){
		a=0;
		b=0;
	}
	Complex operator+(const Complex& z){
		Complex tmp;
		tmp.a=a+z.a;
		tmp.b=b+z.b;
		return tmp;
	}
	Complex operator-(const Complex& z){
		Complex tmp;
        tmp.a=a-z.a;
		tmp.b=b-z.b;
		return tmp;
	}
	Complex operator*(const Complex& z){
		Complex tmp;
		tmp.a = (a)*(z.a) - (b)*(z.b);
		tmp.b = (a)*(z.b) + (b)*(z.a);
		return tmp;
	}
	Complex complement(){
		Complex tmp;
		tmp.a=a;
		tmp.b=-b;
		return tmp;
	}
	Complex operator/( Complex z){
		double den = (z.a)*(z.a)+(z.b)*(z.b);
		Complex tmp;
		tmp.a=a;
		tmp.b=b;
		tmp=tmp*(z.complement());
		tmp.a /= den;
		tmp.b /= den;
		return tmp;
	}
	Complex Inverse(){
		double num=sqrt(a*a+b*b);
		Complex tmp;
		tmp.a=a/num;
		tmp.b=-b/num;
		return tmp;
	}
	void print(){
		if(b>=0)
            cout<<a<<"+"<<b<<"i"<<endl;
        else
            cout<<a<<"-"<<-b<<"i"<<endl;
		return ;
	}
public:
	double a;		//real part
	double b;		//Imaginary part
};

int main(){
	Complex num1;
	Complex num2;
	cout<<"Enter 2 complex numbers : "<<endl;
	cout<<"--Enter 1st number : "<<endl;
	cin>>num1.a>>num1.b;
	cout<<"You entered ";
        num1.print();
	cout<<"--Enter 2nd number : "<<endl;
	cin>>num2.a>>num2.b;
	cout<<"You entered ";
        num2.print();
	cout<<"----------------------------------------------------"<<endl;
	cout<<"Inverse of 1st number is ";
		(num1.Inverse()).print();
	cout<<"Inverse of 2nd number is ";
		(num2.Inverse()).print();
	cout<<"Addition of two numbers (a+b) : ";
		(num1+num2).print();
	cout<<"Subtraction of two numbers (a-b) : ";
		(num1-num2).print();
	cout<<"Multiplication of two numbers (a*b) : ";
		(num1*num2).print();
	cout<<"Division of two numbers (a/b) : ";
		(num1/num2).print();
	cout<<"----------------------------------------------------"<<endl;
    
	return 0;
}
