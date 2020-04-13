#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "FileLoader.h"

/**
 * Clears the contents of this loader so that it can be used again.
 */
void FileLoader::clear() {
    filePathPrefix_ = "";
    loaded_ = false;
}

/**
 * Attempts to process the file with the given path into the loader's memory.
 *
 * @param filePath The path of the file to be loaded.
 * @throws invalid_argument if the given file path contains malformed data and
 *                          cannot be fully loaded, or if the file simply
 *                          cannot be opened.
 */
void FileLoader::loadFile(const std::string& filePath) {
    FileLoader::initFilePathPrefix(filePath);

    std::ifstream file;
    std::cout << filePath << std::endl;
    file.open(filePath);
    if (file.is_open()) {
        std::string line;

        while (getline(file, line)) {
            try {
                processLine(line);
            } catch (std::exception& ex) {
                std::cout << std::endl << ex.what() << std::endl;
                throw std::invalid_argument("The given file path contains malformed"
                                            " data and cannot be fully loaded.");
            }
        }

        file.close();
        loaded_ = true;
    } else {
        std::string message = "Unable to open file at: \"" + filePath + "\".";
        std::cout << message << std::endl;
        throw std::invalid_argument(message);
    }
}

/**
 * Splits a string into constituent pieces by the given delimiter.
 * 
 * @param line  The string to be split into constituent pieces.
 * @param delim The delimiter to separate out the constituent pieces of line.
 * @return A vector containing the  pieces split apart from line.
 */
QVector<std::string> FileLoader::split(const std::string& line, char delim) {
    QVector<std::string> splitLine;
    std::stringstream stream(line);
    std::string item;

    while (getline(stream, item, delim)) {
        splitLine << trim(item);
    }

    return splitLine;
}

/**
 * Trims whitespace from the front and back of the given string.
 *
 * @param string The string to be trimmed.
 * @return The trimmed string.
 */
std::string FileLoader::trim(const std::string& string) {
    std::string whitespace = " \n\r\t\f\v";
    size_t start = string.find_first_not_of(whitespace);
    size_t end = string.find_last_not_of(whitespace);

    if (start == std::string::npos) {
        return "";
    }

    std::string rstring = string.substr(start);

    if (end == std::string::npos) {
        return "";
    }

    return rstring.substr(0, end + 1);
}

/**
 * Parses the given path to determine the directory in which the file sits.
 *
 * @param filePath The path of the file to be loaded.
 */
void FileLoader::initFilePathPrefix(const std::string& filePath) {
    QVector<std::string> filePathSplit = FileLoader::split(filePath, '/');
    std::string fileName = filePathSplit.at(filePathSplit.size() - 1);
    size_t fileNameLoc = filePath.rfind(fileName);
    filePathPrefix_ = filePath.substr(0, fileNameLoc);
}