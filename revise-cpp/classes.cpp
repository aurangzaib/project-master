#include <iostream>
using namespace std;

// http://www.cplusplus.com/doc/tutorial/classes/
// class is like a type and instance is like variable of that type

// first class
class SomeOtherClass {
private:
    int firstProperty;
public:
    SomeOtherClass(int x): firstProperty(x) {}
};

// second class, it will use first class
class someClass {
private:
    // accessible from memebers and friends
    // instances can't access
    SomeOtherClass otherClassProperty;
    int property1, property2;
protected:
    // accessible from members, friends and derived classes
    // instances can't access
    float property3;
public:
    // accessible from instances
    void setProperties (int = 0, int = 0, int = 0, int = 0);
    int getProperties (void);
    SomeOtherClass getOtherProperty (void);
    // ctor overloading
    someClass ();
    someClass (int);
    someClass (int, int);
    someClass (int, int, int);
};

// default ctor init
// var of other classes can only be init using member initialization list method
someClass :: someClass (): otherClassProperty(5) {
    property1 = 5 ;
    property2 = 5 ;
    property3 = 5 ;
}
// fucntional ctor init
someClass :: someClass (int x): otherClassProperty(5) {
    property1 = x ;
    property2 = 5 ;
    property3 = 5 ;
}
// combination of func and member initialization of ctor
someClass::someClass(int x, int y):property1(x), property2(y), otherClassProperty(5) {
    property3 = 5;
}
// member initialization of ctor
someClass :: someClass (int x, int y, int z): property1(x), property2(y), property3(5), otherClassProperty(z) {};

// setter method out of class with resolution operator
void someClass::setProperties(int a, int b, int c, int d) { //
    otherClassProperty = d;
    property1 = a;
    property2 = b;
    property3 = c;
}

// getter method out of class with resolution operator
int someClass::getProperties(void) {
    return property1;
}

SomeOtherClass someClass::getOtherProperty (void) {
    return otherClassProperty;
}


// using above class
void playWithClass (void) {
    
    // functional form of ctor calling
    someClass anInstance; // 5,5,5
    someClass secondInstance(8);// 8,5,5
    someClass thirdInstance(2, 4, 8);
    
    // uniform form of ctor calling
    someClass anInstance2 = {};
    someClass secondInstance2 = {8};
    someClass thirdInstance2 = {6, 7, 8};
    
    // someClass anInstance(); --> invalid, () shouldn't be used for default ctor
    anInstance.setProperties(1, 2, 3);
    
    cout << "1st instance property: " << anInstance.getProperties() << endl
    << "2nd instance property: " << secondInstance.getProperties() << endl
    << "3rd instance property: " << thirdInstance.getProperties() << endl;
}