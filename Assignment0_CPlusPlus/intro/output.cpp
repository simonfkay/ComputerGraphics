#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    std::ofstream outFile;
    outFile.open("temp.txt");
    
    outFile << "# Some image data" << std::endl;
    outFile << "P3" << std::endl;

    outFile.close();

    return 0;
}
