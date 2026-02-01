#include <iostream>

void myFunctionArray(int myArray[5]) {
    // for (int i = 0; i < 5; i++) {
    //     std::cout << myArray[i] << "\n";
    for (int num: myArray) {
        std::cout << num << "\n";
    }

}

int main () {

    int myArray[5] = {1, 2, 3, 4, 5};
    myFunctionArray(myArray);

    return 0;

}