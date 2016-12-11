#include "stdafx.h"
#include <iostream>
using namespace std;

// http://www.cplusplus.com/doc/tutorial/templates/

class ComplexNumbers {
private:
    // static means all object of this class share the same property
    float x, y;
public:
    static int staticProperty;
    ComplexNumbers (): x(1), y(1.5) {}
    ComplexNumbers (float x, float y): x(x), y(y) {};
    float sum (void) {return x + y;}
    
    bool operator == (const ComplexNumbers &);
    ComplexNumbers& operator = (const ComplexNumbers &);
    
    // cant be defined outside of the class
    friend ostream &operator << (ostream &out, const ComplexNumbers number) {
        out << "x: " << number.x << ", ";
        out << "y: " << number.y;
        return out;
    }
    
    ComplexNumbers operator + (const ComplexNumbers &params) {
        x += params.x; // or this->x = params.x
        y += params.y; // or this->y = params.y
        return *this;
    }
    
    ComplexNumbers &operator ++ () {
        ComplexNumbers temp;
        x ++;
        y ++;
        return *this;
    }
};

// static properties must be init outside of the class
int ComplexNumbers::staticProperty=0;

bool ComplexNumbers::operator== (const ComplexNumbers &aNumber){
    if (this == &aNumber) return true; return false;
};

ComplexNumbers& ComplexNumbers::operator= (const ComplexNumbers &aNumber) {
    this->x = aNumber.x;
    this->y = aNumber.y;
    return *this;
};

void playWithClassOverloading () {
    ComplexNumbers number1 (2, 3), number2 (4, 5);
    
    cout << "number1 sum before addition: " << number1.sum() << endl;
    number1 = number1 + number2;
    cout << "number1 sum after addition: " << number1.sum() << endl;
    cout << "isEqual: " << (number1 == number2) << endl;
    
    number1 = number2;
    cout << "number1: " << number1 << endl;
    
    // access the static property from class
    ComplexNumbers::staticProperty = 99;
    cout << "static property thru class: " << ComplexNumbers::staticProperty << endl;
    
    // access the static property from instance
    number1.staticProperty -= 1;
    cout << "static property thru instance: " << number1.staticProperty << endl;

    // number1.staticProperty is same as ComplexNumbers::staticProperty
}