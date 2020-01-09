#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
    std::ifstream inFile;

    if (argc != 2) {
        std::cout << "Oops, exactly two arguments needed.\n";
        std::cout << "ProgramName file_path.\n";
        return 0;
    }

    inFile.open(argv[1]);
    
    if (inFile.is_open()) {
        std::string line;
        while (getline(inFile,line)) {
            std::cout << line << std::endl;
        }
    }
    
    inFile.close();

    return 0;
}