#include <iostream>
#include <string>
#include <vector>
#include <cctype>

// Helper: digits only - it will check to see if user enters only numbers as id not words
bool isAllDigits(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Helper: letters and spaces only -  it will check to see if user does not enter numbers
bool isAllLetters(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

// Helper: convert to uppercase will convert data entered by user to upper case to remove mismatch confusion
// std::string toUpper(std::string s) {
//     for (char& c : s) {
//         c = std::toupper(c);
//     }
//     return s;
// }

//  Employee ID validation
int getValidEmployeeId() {
    std::string input;
    /*entered string cause it will check if any un-necessary words entered by user if not
     *our code will break if user enters "28jgh" instead of 28 it will be read here*/

    while (true) {
        std::cout << "Enter Employee ID: ";
        std::cin >> input;

        if (!isAllDigits(input)) {
            /* we validated the string we have given above if user enters "28hgyd" it will be verified here and we call upon the
             * function isAllDigits and prove it with ! not operator - we are passing the input data entyered by user to above function and validating*/
            std::cout << "Invalid ID. Numbers only.\n";
            continue;
        }

        int id = std::stoi(input); // since we stared with string to verify user input we need a conversion of string to int when user enters correct integer value

        if (id <= 0) {              // making sure user enters positive integer
            std::cout << "Employee ID must be positive.\n";
            continue;
        }
        return id;
    }
}

// Name validation
std::string getValidName() {
    std::string name;

    std::cin.ignore(); //its is to support the getline code to remove the enter option after user enters the input
    while (true) {
        std::cout << "Enter Employee Name: ";
        std::getline(std::cin, name);
        /*we are using getline([&]std --> instead of cin>>name bacause th euser input can be more the multiple charaters
        like std::cin >> name reads only "John" "Doe" stays in the buffer.*/

        if (!isAllLetters(name)) {
            std::cout << "Name must contain letters only.\n";
            continue;
        }

        return name;
    }
}

//  Department validation
std::string getValidDepartment() {
    std::string dept;

    while (true) {
        std::cout << "Enter Department: ";
        std::getline(std::cin, dept);

        if (!isAllLetters(dept)) {
            std::cout << "Department must contain letters only.\n";
            continue;
        }

        return dept;
    }
}

//Salary validation
double getValidSalary() {
    std::string inputSalary;

    while (true) {
        std::cout << "Enter Salary: ";
        std::cin >> inputSalary;

        bool valid = true;
        int dotCount = 0;

        for (char c : inputSalary) {
            if (c == '.') {
                dotCount++;
            }
            else if (!std::isdigit(c)) {
                valid = false;
            }
        }

        if (!valid || dotCount > 1) {
            std::cout << "Invalid salary format.\n";
            continue;
        }

        double salary = std::stod(inputSalary);

        if (salary <= 0) {
            std::cout << "Salary must be greater than zero.\n";
            continue;
        }

        return salary;
    }
}

//  Employee structure
struct Employee {
    int id;
    std::string name;
    std::string department;
    double salary;
};

// Display employee
void displayEmployee(const Employee& e) {
    std::cout << "ID: " << e.id
        << " | Name: " << e.name
        << " | Department: " << e.department
        << " | Salary: $" << e.salary << "\n";
}

//  Main Program
int main() {
    std::vector<Employee> employees;
    int choice;

    while (true) {
        std::cout << "\n===== Employee Registration Menu =====\n";
        std::cout << "1. Register Employee\n";
        std::cout << "2. Display All Employees\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            Employee emp;
            emp.id = getValidEmployeeId();
            emp.name = getValidName();
            emp.department = getValidDepartment();
            emp.salary = getValidSalary();

            employees.push_back(emp);
            std::cout << " Employee registered successfully.\n";
        }
        else if (choice == 2) {
            if (employees.empty()) {
                std::cout << " No employees found.\n";
            }
            else {
                std::cout << "\n--- Employee List ---\n";
                for (const auto& e : employees) {
                    displayEmployee(e);
                }
            }
        }
        else if (choice == 3) {
            std::cout << " Exiting application.\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
