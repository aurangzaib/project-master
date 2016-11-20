#include "basic.cpp"
#include "pointers.cpp"
#include "classes.cpp"

using namespace std;

int main () {
    
#if false
    playWithStrings();
#endif
    
#if false
    playWithArrays();
#endif
    
#if false
    playWithFunctions();
#endif
    
#if false
    playWithPointers();
#endif
    
#if false
    playWithPointerToPointer();
#endif
    
#if true
    
    // functional form of ctor calling
    someClass anInstance; // 5,5,5
    someClass secondInstance(8);// 8,5,5
    someClass thirdInstance(2, 4, 8);
    
    // uniform form of ctor calling
    someClass anInstance2 = {};
    someClass secondInstance2 = {8};
    someClass thirdInstance2 = {6, 7, 8};
    
    // someClass anInstance(); --> invalid, () shouldn't be used for default ctor
    anInstance.setProperties(1,2,3);
    
    cout << "1st instance property: " << anInstance.getProperties() << endl
         << "2nd instance property: " << secondInstance.getProperties() << endl
         << "3rd instance property: " << thirdInstance.getProperties() << endl;
#endif
    return 0;
}
