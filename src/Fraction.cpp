/*
 *	author : Lovekesh Garg
 *	
 *	Fraction Class Implementation in Cpp
 *
 */

#include <iostream>
#include "Fraction.h"

using namespace std;

Fraction(int num_= 1, int den_= 1):iNumerator_(num_),uiDenominator_(den_){							// Uses default parameters. Overloads to
													// Fraction(int, int){} 
	this->Normalize();												// Fraction(int){}	
													// Fraction(){} 
}	
								
// Constructor for explicit Conversion
Fraction(double val){									// explicit double to Fraction conversion
	int num_ = long(val * 1000000);
	iNumerator_ = num_;
	uiDenominator_ = 1000000;
}			

// Copy Constructor
Fraction(const Fraction& tmp){							// Param cannot be changed (const)
	this->iNumerator_ = tmp.GetNumerator();
	this->uiDenominator_ = tmp.GetDenominator();
}	

// DESTRUCTOR
// ----------
~Fraction(){
	delete sc_fUnity;
	delete sc_fZero;
}	// No virtual destructor needed

// BASIC ASSIGNEMENT OPERATOR
Fraction& operator=(const Fraction& tmp){
	iNumerator_ = tmp.GetNumerator();
	uiDenominator_ = tmp.GetDenominator();
	return Fraction(iNumerator_, uiDenominator_);
}

// UNARY ARITHMETIC OPERATORS
// --------------------------
Fraction operator-(){							// Operand 'this' implicit
	iNumerator_ *= -1;
	return Fraction(iNumerator_, uiDenominator_);
}		
Fraction operator+(){
	iNumerator_ *= 1;
	return Fraction(iNumerator_, uiDenominator_);

}
Fraction& operator--(){							// Pre-decrement. Dividendo. p/q <-- p/q - 1
	iNumerator_ -= uiDenominator_;
	return Fraction(iNumerator_, uiDenominator_);
}		
Fraction& operator++(){							// Pre-increment. Componendo. p/q <-- p/q + 1
	iNumerator_ += uiDenominator_;
	return Fraction(iNumerator_, uiDenominator_);
}		
Fraction operator--(int){						// Post-decrement. Lazy Dividendo. p/q <-- p/q - 1. Returns old p/q
	iNumerator_ -= uiDenominator_;
	return Fraction(iNumerator_ + uiDenominator_, uiDenominator_);
}	
Fraction operator++(int){						// Post-increment. Lazy Componendo. p/q <-- p/q + 1. Returns old p/q
	iNumerator_ += uiDenominator_;
	return Fraction(iNumerator_ - uiDenominator_, uiDenominator_);
}	

// BINARY ARITHMETIC OPERATORS USING FRIEND FUNCTIONS
// --------------------------------------------------
Fraction operator+(const Fraction& a, const Fraction& b){
	int num_ = a.iNumerator_ * b.uiDenominator_ + b.iNumerator_ * a.uiDenominator_;
	int den_ = a.uiDenominator_* b.uiDenominator_;
	return Fraction(num_, den_);
}
Fraction operator-(const Fraction& a, const Fraction& b){
	int num_ = a.iNumerator_ * b.uiDenominator_ - b.iNumerator_ * a.uiDenominator_;
	int den_ = a.uiDenominator_ * b.uiDenominator_;
	return Fraction(num_, den_);
}
Fraction operator*(const Fraction& a, const Fraction& b){
	int num_ = a.iNumerator_ * b.iNumerator_;
	int den_ = a.uiDenominator_ * b.uiDenominator_;
	return Fraction(num_, den_);
}
Fraction operator/(const Fraction& a, const Fraction& b){
	int num_ = a.iNumerator_ * b.uiDenominator_;
	int den_ = a.uiDenominator_ + b.iNumerator_;
	return Fraction(num_, den_);
}
Fraction operator%(const Fraction& a, const Fraction& b){

}

// BINARY RELATIONAL OPERATORS
// ---------------------------
bool operator==(const Fraction& frc){
	return (iNumerator_ * frc.uiDenominator_ == uiDenominator_ * frc.iNumerator_);
}
bool operator!=(const Fraction& frc){
	return (iNumerator_ * frc.uiDenominator_ != uiDenominator_ * frc.iNumerator_);
}
bool operator<(const Fraction& frc){
	return (iNumerator_ * frc.uiDenominator_ < uiDenominator_ * frc.iNumerator_);
}
bool operator<=(const Fraction& frc){
	return (iNumerator_ * frc.uiDenominator_ <= uiDenominator_ * frc.iNumerator_);
}
bool operator>(const Fraction&){
	return (iNumerator_ * frc.uiDenominator_ > uiDenominator_ * frc.iNumerator_);
}
bool operator>=(const Fraction&){
	return (iNumerator_ * frc.uiDenominator_ >= uiDenominator_ * frc.iNumerator_);
}

// ADVANCED ASSIGNEMENT OPERATORS
// ------------------------------
Fraction& operator+=(const Fraction& frc){
	iNumerator_ = iNumerator_ * frc.uiDenominator_ + frc.iNumerator_ * uiDenominator_;
	uiDenominator_ = uiDenominator_* frc.uiDenominator_;
	return Fraction(iNumerator_, uiDenominator_);
}
Fraction& operator-=(const Fraction&){
	iNumerator_ = iNumerator_ * frc.uiDenominator_ - frc.iNumerator_ * uiDenominator_;
	uiDenominator_ = uiDenominator_* frc.uiDenominator_;
	return Fraction(iNumerator_, uiDenominator_);
} 
Fraction& operator*=(const Fraction& frc){
	iNumerator_ = iNumerator_ * frc.iNumerator_;
	uiDenominator_ = uiDenominator_ * frc.uiDenominator_;
	return Fraction(iNumerator_, uiDenominator_);
}
Fraction& operator/=(const Fraction& frc){
	iNumerator_ = iNumerator_ * frc.uiDenominator_;
	uiDenominator_ = frc.uiDenominator_ + iNumerator_;
	return Fraction(iNumerator_, uiDenominator_);
}
Fraction& operator%=(const Fraction& frc){

}

// SPECIAL OPERATORS
// -----------------
Fraction operator!(){											
	swap(iNumerator_, uiDenominator_);
	return Fraction(iNumerator_, uiDenominator_);

}		

// BASIC I/O using FRIEND FUNCTIONS
// --------------------------------
ostream& operator<<(ostream& out, const Fraction& frc){
	out << " "<<iNumerator_ << "/" << uiDenominator_ << endl;
	return out;
}
istream& operator>>(istream& inp, Fraction& frc){
	inp >> this->iNumerator_ >> this->uiDenominator_;
	return inp;
}

// CONSTANTS OF DATATYPE
// ---------------------
sc_fUnity = new Fraction(1, 1);									// Defines 1/1

sc_fZero = new Fraction(0, 1);									// Defines 0/1

// STATIC UTILITY FUNCTIONS
// ------------------------
static const int precision(){ return 1000000; }
static int gcd(int a, int b){											// Finds the gcd for two +ve integers
	if(a < b){
		swap(a,b);
	}
	if(b == 0)
		return a;
	return gcd(b, a%b);
}	
static int lcm(int a, int b){											// Finds the lcm for two +ve integers
	return a*b/gcd(a, b);
}	
static int swap(int& a, int& b){
	int t = a;
	a = b;
	b = t;
	return;
}
// COMPONENT FUNCTIONS
//--------------------
int GetNumerator() { return iNumerator_; }
unsigned int GetDenominator() { return uiDenominator_; }

// OTHER METHOD MEMBERS
// --------------------
Fraction& Normalize(){												// Normalizes a fraction
	int tmp = gcd(iNumerator_, uiDenominator_);
	iNumerator_ /= tmp;
	uiDenominator_ /= tmp;
	Fraction frc(iNumerator_, uiDenominator_);
	return frc;
}	