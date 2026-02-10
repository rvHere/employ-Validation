#include <iostream>
#include <ctime>

void functionAfterMain(int a, int b);

void addFunction(int a, int b){
        std::cout << "the output is: "<< a+b << "\n";
        std::time_t now = std::time(nullptr);
        std::cout << "Time now is: " << std::ctime(&now);
}

void testFunction(int a, int b) {
        std::cout << "the output is: " << a+b << "\n";
}

void subtractFunction(int a, int b){
        std::cout << "the subtraction is: " << a-b << "\n";
        std::time_t now = std::time(nullptr);
        std::cout << "Time now is: " << std::ctime(&now);
}

void multiplyFunction(int a, int b){
        std::cout << "the Multiplication is: " << a*b << "\n";
        std::time_t now = std::time(nullptr);
        std::cout << "Time now is: " << std::ctime(&now);
}

void divisionFunction(double a, int b){
        std::cout << "the division is: " << a/b << "\n";
        std::time_t now = std::time(nullptr);
        std::cout << "Time now is: " << std::ctime(&now);
}

int main(){

        addFunction(10,20);
        subtractFunction(10,20);
        multiplyFunction(10,20);
        divisionFunction(10,20);
        functionAfterMain(10,20);
        return 0;
}

void functionAfterMain(int a, int b){
     std::cout << "This function is after main function and output: " << a+b << "\n";

}


 