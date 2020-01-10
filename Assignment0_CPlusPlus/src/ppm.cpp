#include <fstream>
#include <iostream>

#include "ppm.h"

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
    std::vector<std::string> fileContents = PPM::loadFile(fileName);
    PPM::fetchMetadata(fileContents);

    //std::cout << (*metadata)[0] << std::endl;
    //delete metadata;
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
void fetchMetadata(std::vector<std::string> fileData) {
    std::cout << "Fetching metadata." << std::endl;
    std::vector<unsigned char> returnValue;
/**
    // Files must have a properly formatted header (does not check formatting,
    // just assumes it)
    if (fileData.size() < 4) {
        std::cout << "File must have a proper 4 line header." << std::endl;
        return returnValue;
    }
    
    // Read lines and break them down
    std::string sizeData = fileData[2];
    std::string maxValue = fileData[3];


    std::vector<std::string> separateWords = separateWords(sizeData);

    std::cout << "Metadata:" << std::endl;

    for (int ii = 0; ii < separateWords.size(); ++ii) {
        std::cout << separateWords[ii] << std::endl;
    }
    

    // TODO: Error: if separateWords isn't 2 long

    //std::string width = separateWords[0];
    //std::string height = separateWords[1];

    // TODO: Parse strings for unsigned integers:
    //         - Test: Is stoi the correct method here?

    int width = atoi("3");

    unsigned char widthChar = stoi(width);
    unsigned char heightChar = stoi(height);
    unsigned char maxValueChar = stoi(maxValue);

    // End TODO

    returnValue.push_back(widthChar);
    returnValue.push_back(heightChar);
    returnValue.push_back(maxValueChar);*/
    //return returnValue;
}

/**
std::vector<std::string> separateWords(std::string line) {
    std::cout << std::endl << "Separating Words:" << std::endl;
    std::istringstream stream(line);
    std::vector<std::string> separateWords;
  
    do { 
        std::string word; 
        stream >> word;
        std::cout << "Word: " << word << std::endl;
        separateWords.push_back(word);
    } while (stream); 
    return separateWords;
}

// Convert line strings into usable data
std::vector<std::string> processLines(std::vector<std::string> lineData) {

}
*/
