#include <iostream>
using namespace std;

// http://www.cplusplus.com/doc/tutorial/classes/

class Rectangle {
private:
    float height, width;
public:
    Rectangle (float height, float width): height(height), width (width) {}
    float area(void) {
        return height * width;
    };
};


void playWithClassPointer (void) {
    Rectangle obj (2.5, 3.5);
    Rectangle *pointer1;
    Rectangle *pointerArray[2];
    
    pointer1 = new Rectangle(4.5, 1.2);
    pointerArray[0] = new Rectangle (1.2, 3.4);
    pointerArray[1] = new Rectangle (0.2, 0.4);
    
    cout << "obj area: " << obj.area() << endl;
    cout << "pointer 1 area: " << pointer1 -> area() << endl;
    cout << "pointer array 0 area: " << pointerArray[0]->area() << endl;
    
    delete pointer1;
    delete pointerArray[1];
    
    // pointer is removed
    cout << "pointer 1 area: " << pointer1 -> area() << endl;
}


// struct can be used to define a class
// when we use 'class', then members are private by default
// when we use 'struct', then members are public by default