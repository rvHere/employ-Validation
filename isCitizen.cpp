//
// Created by chand on 1/27/2026.
//
#include  <iostream>
#include <string>
#include <cctype>
#include <vector>

//helper for digits
bool isAllDigits(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

//helper for strings
bool isAllLetters(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

// Helper: convert to lowercase
// std::string toLower(std::string s) {
//     for (char& c : s) {
//         c = std::tolower(c);
//     }
//     return s;
// }

//getting age from user with validation
int getValidAge() {
    std::string inputAge;

    while (true) {
        std::cout << "Enter Your Age: ";
        std::cin >> inputAge;

        if (!(isAllDigits(inputAge))) {
            std::cout << "Invalid Input. Please try again." << "\n";
            continue;
        }

        int age = std::stoi(inputAge);

        if (age <= 0) {
            std::cout << "age musty be positive.\n";
            continue;
        }
        if (age<18) {
            std::cout << "age less than 18 - Cant Vote \n";
            continue;
        }
        if (age >= 18) {
            std::cout << "age is correct to vote\n";
            return age;   // exit the loop and function
        }

    }
}

std::initializer_list<char> toLower(const std::string & string);

std::string isCitizenValidation() {
    std::string inputCitizen;

    while (true) {
        std::cout << "Enter Your Citizen - Yes or No: ";
        std::cin >> inputCitizen;

        if (!(isAllLetters(inputCitizen))) {
            std::cout << "Invalid Input. Please try again." << "\n";
            continue;
        }
        inputCitizen = toLower(inputCitizen);


    }

}

int main() {



    return 0;
}