#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    std::vector<unsigned char> myData;
    
    for (unsigned int ii = 65; ii < 91; ++ii) {
        myData.push_back(ii);
    }

    for (unsigned int ii = 0; ii < myData.size(); ++ii) {
        std::cout << myData[ii];
    }

    return 0;
}
