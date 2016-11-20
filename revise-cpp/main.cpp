/*
 namespace is used to isolate scope of different libraries
 so that two libs can have same func and variable names
 without interfering scopes of other libs
 
 http://www.cplusplus.com/doc/tutorial/namespaces/
 */

/*
 automatic storage - for example local variabel
 static storage - for example global variable
 static storage is by default init to 0 while automatic storage is uninit by default
 */

#include <iostream>
#include <string>
using namespace std;

// using variable by address
void someFunc (int &a, int &b, int c=3) { // here default value for c is 3
    a+=b;
    c+=2;
}


// inline functions
inline void someInlineFunc (int a ){
    // do something here
    // inline tells compiler that put the fn code in main instd of invoking it
    // good for short functions
}


// fucntion overloading
void overloaded (int a){
    cout << "int one is called" << endl;
}
void overloaded(float a, float b){
    cout << "float one is called" << endl;
}

// template -- function which allows different types
template <class T>
T templateFunc (T a, T b){
    return a+b;
}

int main () {
    int a=2, b=2, c=2;
    
    someFunc(a, b, c);
    
    cout << "value of a now: " << a << endl;
    cout << "value of c now: " << c << endl;
    overloaded(12); // int one
    overloaded(3.2323, 2.3); // float one
    
    cout << "templateFunc with integers: " << templateFunc((int)1, (int)2.3) << endl;
    cout << "templateFunc with floats: " << templateFunc(1.333, 2.3) << endl;
    
    // matrix
    int myMatrix[2][3]; // 2x3 matrix
    
    // number of rows
    int numberOfRows = sizeof(myMatrix)/sizeof(myMatrix[0]) ;
    // number of columns
    int numberOfColumns = sizeof(myMatrix[0])/sizeof(int) ;
    //for (int row=0; row<)
    cout << "rows: " << numberOfRows << endl << "columns: " << numberOfColumns << endl;
    for (int row = 1; row <= numberOfRows; row++) {
        for (int column = 1; column <= numberOfColumns; column++) {
            myMatrix[row][column] = row * column;
            cout << myMatrix[row][column] << "  " ;
        }
        cout << endl;
    }
    
    string someString  = "go to home with string";
    char someCharString[] = "go to home with char";
    cout << "with string: " << someString
         << endl
         << "with char: " << someCharString << endl;
    
    return 0;
}