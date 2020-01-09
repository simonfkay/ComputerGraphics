#include <iostream>
#include <string>

int main() {
    std::string fullName = "Simon Kay";
    std::string firstName = fullName.substr(0,5);
    std::string lastName = fullName.substr(6,9);

    std::cout << "First: " << firstName << std::endl;
    std::cout << "Last: " << lastName << std::endl;

    unsigned int ii = 0;
    while (ii < fullName.length()) {
        if (fullName.at(ii)==' ') {
            break;
        }

        std::cout << fullName[ii];
        ++ii;
    }

    return 0;
}