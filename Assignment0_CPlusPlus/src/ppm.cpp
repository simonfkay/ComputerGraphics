#include <fstream>
#include <iostream>
#include <sstream>

#include "PPM.h"

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
    std::vector<std::string> fileContents = PPM::loadFile(fileName);
    PPM::processLines(fileContents);
}

// Destructor clears any memory that has been allocated
PPM::~PPM() {
    delete[] this->m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) {
    if (!this->m_PixelData) {
        std::cerr << "PPM data improperly loaded, cannot save." << std::endl;
        return;
    }

    std::ofstream outFile;
    outFile.open(outputFileName);
    outFile << PPM::toString();
    outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken() {
    if (!this->m_PixelData) {
        std::cerr << "PPM data improperly loaded, cannot darken." << std::endl;
        return;
    }

    unsigned int size = this->m_width * this->m_height * 3;
    for (unsigned int ii = 0; ii < size; ++ii) {
        this->m_PixelData[ii] = std::max(0, this->m_PixelData[ii] - 50);
    }
    return;
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B) {
/*    if (!this->m_PixelData) {
        std::cerr << "Pixel data must first be initialized." << std::endl;
        return;
    }*/

    unsigned int redIndex = y * this->m_width * 3 + x * 3;

    if (redIndex + 2 > (this->m_width * this->m_height * 3)) {
        std::cerr << "Cannot edit data at given indices; out of bounds." << std::endl;
        return;
    }

    this->m_PixelData[redIndex] = R;
    this->m_PixelData[redIndex + 1] = G;
    this->m_PixelData[redIndex + 2] = B;
}

// Load the desired file into raw strings for each line
std::vector<std::string> PPM::loadFile(std::string fileName) {
    std::vector<std::string> fileLines;
    std::ifstream inFile;

    inFile.open(fileName);

    if (inFile.is_open()) {
        std::string line;
        while (getline(inFile, line)) {
            fileLines.push_back(line);
        }
    }
    
    inFile.close();
    return fileLines;
}

// Process metadata, and then load image data into program
void PPM::processLines(std::vector<std::string> fileContents) {
    bool checkedVersion = false;
    bool checkedDims = false;
    bool checkedMax = false;

    std::vector<int> numbers;
    for (unsigned int ii = 0; ii < fileContents.size(); ++ii) {
        std::string line = fileContents[ii];

        if (PPM::isComment(line)) {
            continue;
        }

        if (!checkedVersion) {
            PPM::checkVersion(line);
            checkedVersion = true;
            continue;
        }

        if (!checkedDims) {
            PPM::getDims(line);
            checkedDims = true;
            continue;
        }

        if (!checkedMax) {
            PPM::getMax(line);
            checkedMax = true;
            continue;
        }

        std::vector<int> lineNumbers = PPM::getImageLine(line);
        for (int jj = 0; jj < lineNumbers.size(); ++jj) {
            numbers.push_back(lineNumbers[jj]);
        }
    }

    PPM::setPixels(numbers);
}

bool PPM::isComment(std::string line) {
    if (line.size() == 0) {
        std::cerr << "PPM files should not have any empty lines." << std::endl;
        throw -1;
    }
    return line.at(0) == '#';
}

bool PPM::checkVersion(std::string line) {
    if (line != "P3") {
        std::cerr << "Only P3 .ppm files can be loaded." << std::endl;
        throw -1;
    }
    return true;
}

void PPM::getDims(std::string line) {
    std::vector<std::string> words = PPM::separateWords(line);

    if (words.size() != 2) {
        std::cerr << "Line specifying image dimensions must contain two integers." << std::endl;
        throw -1;
    }

    this->m_width = stoi(words[0]);
    this->m_height = stoi(words[1]);
}

void PPM::getMax(std::string line) {
    std::vector<std::string> words = PPM::separateWords(line);

    if (words.size() != 1) {
        std::cerr << "Line specifying maximum pixel value must contain one integer." << std::endl;
        throw -1;
    }

    this->m_maxVal = stoi(words[0]);
}

std::vector<int> PPM::getImageLine(std::string line) {
    std::vector<std::string> words = PPM::separateWords(line);
    std::vector<int> numbers;

    for (unsigned int ii = 0; ii < words.size(); ++ii) {
        numbers.push_back(stoi(words[ii]));
    }

    return numbers;
}

// Separates string into a vector of words
std::vector<std::string> PPM::separateWords(std::string line) {
    std::vector<std::string> words;

    unsigned int ii = 0;
    while (ii < line.size()) {
        if (line.at(ii) == ' ') {
            ++ii;
            continue;
        }
        unsigned int jj = ii;
        for (; jj < line.size(); ++jj) {
            if (line.at(jj) == ' ') {
                break;
            }
        }
        words.push_back(line.substr(ii, jj));
        ii = jj;
    }

    return words;
}

std::string PPM::toString() {
    std::string asString = "P3\n" +
                           std::to_string(this->m_width) + " " + std::to_string(this->m_height) + "\n" +
                           std::to_string(this->m_maxVal) + "\n";

    for (unsigned int ii = 0; ii < this->m_height; ++ii) {
        for (unsigned int jj = 0; jj < (this->m_width * 3); ++jj) {
            unsigned char entry = this->m_PixelData[ii * 3 * this->m_width + jj];
            asString += " " + std::to_string((int) entry) + " ";

            if (jj % 3 == 2 && jj != this->m_width * 3) {
                asString += "  ";
            }
        }
        asString += "\n";
    }

    return asString;
}

void PPM::setPixels(std::vector<int> numbers) {
    this->m_PixelData = new unsigned char[numbers.size()];

    int numDataPoints = this->m_width * this->m_height * 3;
    if (numbers.size() != numDataPoints) {
        std::cerr << "File should contain " << numDataPoints << " points of data, but it only contains " << numbers.size() << std::endl;
    }

    for (unsigned int ii = 0; ii < this->m_height; ++ii) {
        for (unsigned int jj = 0; jj < this->m_width; ++jj) {
            int redIndex = ii * this->m_width * 3 + jj * 3;
            int red = numbers[redIndex];
            int green = numbers[redIndex + 1];
            int blue = numbers[redIndex + 2];

            PPM::setPixel(jj, ii, red, green, blue);
        }
    }
}




































