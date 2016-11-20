#include <iostream>
using namespace std;

// http://www.cplusplus.com/doc/tutorial/classes/
// class is like a type and instance is like variable of that type
class someClass {
private:
    // accessible from memebers and friends
    // instances can't access
    int property1, property2;
protected:
    // accessible from members, friends and derived classes
    // instances can't access
    float property3;
public:
    // accessible from instances
    void setProperties (int=0, int=0, int=0);
    int getProperties (void);
    // ctor overloading
    someClass ();
    someClass (int);
    someClass (int, int);
    someClass (int, int, int);
};

// default ctor init
someClass :: someClass () {
    property1 = 5 ;
    property2 = 5 ;
    property3 = 5 ;
}
// fucntional ctor init
someClass :: someClass (int x) {
    property1 = x ;
    property2 = 5 ;
    property3 = 5 ;
}
// combination of func and member initialization of ctor
someClass::someClass(int x, int y): property1(x), property2(y){
    property3 = 5;
}
// member initialization of ctor
someClass :: someClass (int x, int y, int z): property1(x), property2(y), property3(z) {};

// setter method out of class with resolution operator
void someClass::setProperties(int a, int b, int c){ //
    property1 = a;
    property2 = b;
    property3 = c;
}

// getter method out of class with resolution operator
int someClass::getProperties(void){
    return property1;
}
