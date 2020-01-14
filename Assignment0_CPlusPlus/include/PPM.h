/** @file PPM.h
 *  @brief Class for working with PPM images
 *  
 *  Class for working with P3 PPM images specifically.
 *
 *  @author Simon Kay
 *  @bug No known bugs.
 */
#ifndef PPM_H
#define PPM_H

#include <string>
#include <vector>

class PPM {
public:
    // Constructor loads a filename with the .ppm extension
    PPM(std::string fileName);

    // Destructor clears any memory that has been allocated
    ~PPM();

    // Saves a PPM Image to a new file.
    void savePPM(std::string outputFileName);

    // Darken subtracts 50 from each of the red, green
    // and blue color components of all of the pixels
    // in the PPM. Note that no values may be less than
    // 0 in a ppm.
    void darken();

    // Sets a pixel to a specific R,G,B value 
    void setPixel(int x, int y, int R, int G, int B);

    // Returns the raw pixel data in an array.
    // You may research what 'inline' does.
    inline unsigned char* pixelData() { return m_PixelData; }

    // Returns image width
    inline int getWidth() { return m_width; }

    // Returns image height
    inline int getHeight() { return m_height; }

// NOTE:    You may add any helper functions you like in the
//          private section.

private:    
    // Store the raw pixel data here
    // Data is R,G,B format
    unsigned char* m_PixelData;

    // Store width and height of image.
    int m_width{0};
    int m_height{0};
    int m_maxVal{0};

    // Load the desired file into a vector of raw strings
    std::vector<std::string> loadFile(std::string fileName);

    // Fetches metadata in first part of line data
    std::vector<int> fetchMetadata(std::vector<std::string> fileData);

    // Separates string into a vector of words
    std::vector<std::string> separateWords(std::string line);

    // Convert line strings into usable data
    void processLines(std::vector<std::string> lineData);

    std::string toString();

    bool isComment(std::string line);

    bool checkVersion(std::string line);

    void getDims(std::string line);

    void getMax(std::string line);

    std::vector<int> getImageLine(std::string line);

    void setPixels(std::vector<int> numbers);
};


#endif
