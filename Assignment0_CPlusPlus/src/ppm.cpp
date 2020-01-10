#include <fstream>
#include <iostream>
#include <sstream>

#include "PPM.h"

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
    std::vector<std::string> fileContents = PPM::loadFile(fileName);
    std::vector<int> metadata = PPM::fetchMetadata(fileContents);
    
    m_width = metadata[0];
    m_height = metadata[1];
    m_maxVal = metadata[2];

    //std::vector<std::string> processedLines = processLines(fileContents);

    //std::cout << fileContents << std::endl;
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

// Load the desired file into a raw string
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

// Fetches metadata in first part of line data
// TODO: Error checking
std::vector<int> PPM::fetchMetadata(std::vector<std::string> fileData) {
    std::vector<int> returnValue;

    // TODO: Check that files have at least 4 lines
    
    // Read relevant lines and break them down
    std::string sizeData = fileData[2];
    std::string maxValue = fileData[3];

    std::vector<std::string> separateWords = PPM::separateWords(sizeData);

    // TODO: Check that size data has 2 words

    std::string width = separateWords[0];
    std::string height = separateWords[1];

    // TODO: Make sure that all of these can be parsed as numbers

    int widthChar = stoi(width);
    int heightChar = stoi(height);
    int maxValueChar = stoi(maxValue);

    returnValue.push_back(widthChar);
    returnValue.push_back(heightChar);
    returnValue.push_back(maxValueChar);
    return returnValue;
}

// Separates string into a vector of words
std::vector<std::string> PPM::separateWords(std::string line) {
    std::istringstream stream (line);
    std::vector<std::string> separateWords;
  
    do { 
        std::string word; 
        stream >> word;
        separateWords.push_back(word);
    } while (stream); 
    return separateWords;
}

/**
// Convert line strings into usable data
std::vector<std::string> processLines(std::vector<std::string> lineData) {

}
*/
