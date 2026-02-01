#include <iostream>
#include <string>

void myFunction(std::string country , int age, std::string timeZone = "CST") {
    std::cout << country << age << timeZone<< "\n";
}

int main() {
    myFunction("USA", 100, "EST");
    return 0;
}

//Output
// India
// USA