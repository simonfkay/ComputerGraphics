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

private:    
    // Store the raw pixel data here
    // Data is R,G,B format
    unsigned char* m_PixelData;

    // Store width and height of image.
    int m_width{0};
    int m_height{0};
    int m_maxVal{0};

    // Verifies that the given line specifies the correct P3 format of PPM files
    bool checkVersion(std::string line);

    // Gets the dimensions of the image from the given line
    void getDims(std::string line);

    // Parses the given line for valid pixel values
    std::vector<int> getImageLine(std::string line);

    // Gets the maximum pixel value of the image from the given line
    void getMax(std::string line);

    // Checks if the given line is a PPM comment (i.e. line starts with '#')
    bool isComment(std::string line);

    // Load the desired file into a vector of raw strings
    std::vector<std::string> loadFile(std::string fileName);

    // Convert line strings into usable data
    void processLines(std::vector<std::string> lineData);

    // Separates string into a vector of words
    std::vector<std::string> separateWords(std::string line);

    // With the given integer values from the file, populates the internal structure of this object to be
    // usable
    void setPixels(std::vector<int> numbers);

    // Converts the current state of this object to a string to be printed back out to a file
    std::string toString();

};

#endif
