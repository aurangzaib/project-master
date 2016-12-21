#include "stdafx.h"
#include <iostream>
using namespace std;

// http://www.cplusplus.com/doc/tutorial/pointers/
void playWithPointers() {
  // a variable which stores the address of another variable is called pointer
  // & -> reference operator
  // * -> dereference operator
  int someVar = 23;
  int *someAddress = &someVar;
  // above, int means the type of variable
  // which pointer is pointed to
  cout << "a/d: " << someAddress << endl;
  cout << "val: " << *someAddress << endl;

  // some thing to keep in mind

  int firstValue = 0, secondValue = 0;
  int *pointToFirstValue, *pointToSecondValue;

  pointToFirstValue = &firstValue;    // point to address of firstValue
  pointToSecondValue = &secondValue;  // point to address of secondValue

  *pointToFirstValue = 10;  // firstValue is now 10
  cout << "firstValue: " << firstValue << endl;

  *pointToSecondValue = 20;  // secondValue is now 20
  cout << "secondValue: " << secondValue << endl;

  *pointToSecondValue =
      *pointToFirstValue;  // firstValue and secondValue have same value
  cout << "firstValue: " << firstValue << " | secondValue: " << secondValue
       << endl;

  pointToSecondValue = pointToFirstValue;  // now both pointing to firstValue

  *pointToSecondValue = 50;
  // will change value of firstValue instd of secondValue
  // equaling pointers doesnt mean that the actual variables are equaled
  // now just both pointers will point to same variable
  // but still both variables will still be different
  cout << "firstValue: " << firstValue << " | secondValue: " << secondValue
       << endl;
}

void playWithPointerAndArray() {
  // pointer and arrays - special combination

  int anArray[3] = {4, 2, 3};
  int *pointToArray;
  pointToArray =
      anArray;  // pointing to array address, note that we didnt use &anArray

  // anArray[3] ; *(pointer+3); *(anArray+3)
  // anArray[0] *anArray --> address of 0th element
  // anArray[5] *(anArray+5) --> value of 5th element
  // pointer+4  anArray+4 &anArray[2]
  // anArray anArray[0] &anArray[0] --> all giving address of 0th element

  // 1st element address
  if (anArray + 1 == pointToArray + 1 && pointToArray + 1 == &anArray[1]) {
    cout << "1st element address: " << anArray << endl;
  };

  // 2nd element value
  if (*(pointToArray + 2) == anArray[2] && *(anArray + 2) == anArray[2]) {
    cout << "2nd element value: " << anArray[2] << endl;
  };

  // 0th element value
  if (*anArray == *pointToArray && *pointToArray == anArray[0]) {
    cout << "0th element value: " << anArray[0] << endl;
  };

  // cout << *(pointToArray++) << endl; --> increment address but show previous
  // value
  // cout << *(++pointToArray) << endl; --> increment address then show new
  // value
  // cout << *(pointToArray+1) << endl; --> increment address then show new
  // value
  // cout << ++(*pointToArray) << endl; --> incrment value of current address

  // remember here that even though pointers and array work in same way
  // still there is one important difference. pointer can point to another
  // variable too but array can't change its address

  // pointer can point to new variable and change its address

  // so this is possible --> pointToArray = &someVar
  // but this is not     --> anArray = &someVar

  // rvalue --> which is readonly
  // lvalue --> which is read/write

  // constant pointers can't change value, just can read it.
  // const int *constantPointer = anArray;

  // constant pointers are not themselves constant, just that they can't change
  // variable to which they point
  // still they point to a new variable
}

int a = 4;                            // variable
int *erstePointer = &a;               // pointer to variable
int **zweitePointer = &erstePointer;  // pointer to pointer

// a; *erstePointer; **zweitePointer --> value of a
// &a; erstePointer; *zweitePoiner --> address of a
// &erstePointer; zweitePointer --> address of erstePointer
// &zweitePointer --> address of zeitePointer

void playWithPointerToPointer() {
  // pointer to a pointer
  // its possible that a pointer points to a pointer which in turn points to a
  // variable
  int a = 4;
  int *b = &a;
  int **c = &b;
  cout << "value of a: " << a << " | address of a is: " << &a << endl;
  cout << "value of b: " << b << " | address of b is: " << &b << endl;
  cout << "value of c: " << c << " | address of c is: " << &c << endl;
  cout << "value of variable to which b points: " << *b
       << " | value of b: " << b << " | address of b: " << &b << endl;
  cout << "value of a through b: " << *b << endl;
  cout << "value of a through c: " << **c << endl;

  // null pointers
  // pointers can point to address which doesn't exist. it will create error
  // but dereferencing this pointer will generate runtime error
  // all null pointers are equal
  int *nullPointer = nullptr;

  cout << "null pointer value: " << nullPointer
       << " | null pointer address: " << &nullPointer << endl;

  // nullPointer is 0x0, &nullPointer is address, *nullPointer is exception
  // error.
}
