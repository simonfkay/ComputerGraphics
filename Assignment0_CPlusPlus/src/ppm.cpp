#include <fstream>
#include <iostream>

#include "ppm.h"

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
    std::string fileContents = loadFile(fileName);
    std::cout << fileContents << std::endl;
}

// Destructor clears any memory that has been allocated
PPM::~PPM() {
    delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) {
    // TODO: Save a PPM image to disk
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken() {
    // TODO: Output a 'filtered' PPM image.
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B) {
    // TODO: Implement
}

std::string PPM::loadFile(std::string fileName) {
    std::string fileContents = "";
    std::ifstream inFile;

    inFile.open(fileName);

    if (inFile.is_open()) {
        std::string line;
        while (getline(inFile, line)) {
            fileContents += line + "\n";
        }
    }
    
    inFile.close();
    return fileContents;
}
