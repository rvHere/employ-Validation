#include <iostream>
#include <string>
#include <vector>

// Car structure Defined and named as Car
struct Car {
    int id;
    std::string brand;
    std::string model;
    int year;
};

// Display car - we displayed car as const so the data cant be modified and referred to c
void displayCar(const Car& c) {
    std::cout << "ID: " << c.id << " | Brand: " << c.brand << " | Model: " << c.model << " | Year: " << c.year << "\n";
}

int main() {
    std::vector<Car> cars; //cars is a vector array of Car structs

    Car c1;
    c1.id = 1;
    c1.brand = "BMW";
    c1.model = "X5";
    c1.year = 1999;

    Car c2;
    c2.id = 2;
    c2.brand = "Ford";
    c2.model = "Mustang";
    c2.year = 1969;

    cars.push_back(c1); // we are pushing above created data into array std::vector<Car>
    cars.push_back(c2);

    std::cout << "\n--- Car List ---\n";
    for (const Car& c : cars) { //range based for loop for displaying Cars Array output
        displayCar(c);
    }

    return 0;
}
