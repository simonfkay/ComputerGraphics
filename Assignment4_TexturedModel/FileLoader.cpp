#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <QVector>

#include "FileLoader.h"

/**
 * Attempts to process the file with the given path into the loader's memory.
 *
 * @param filePath The path of the file to be loaded.
 * @throws invalid_argument if the given file path contains malformed data and
 *                          cannot be fully loaded, or if the file simply
 *                          cannot be opened.
 */
void FileLoader::loadFile(std::string filePath) {
    FileLoader::initFilePathPrefix(filePath);

    std::ifstream file;
    file.open(filePath);
    if (file.is_open()) {
        std::string line;

        while (getline(file, line)) {
            try {
                ObjLoader::processLine(line);
            } catch (std::exception& ex) {
                std::cout << std::endl << ex.what() << std::endl;
                throw std::invalid_argument("The given file path contains malformed"
                                            " data and cannot be fully loaded.");
            }
        }

        file.close();
    } else {
        throw std::invalid_argument("Unable to open file at: " + filePath);
    }
}

/**
 * Splits a string into constituent pieces by the given delimiter.
 * 
 * @param line  The string to be split into constituent pieces.
 * @param delim The delimiter to separate out the constituent pieces of line.
 * @return A vector containing the  pieces split apart from line.
 */
QVector<std::string> FileLoader::split(std::string line, char delim) {
    QVector<std::string> splitLine;
    std::stringstream stream(line);
    std::string item;

    while (getline(stream, item, delim)) {
        splitLine << item;
    }

    return splitLine;
}

/**
 * Parses the given path to determine the directory in which the file sits.
 *
 * @param filePath The path of the file to be loaded.
 */
void FileLoader::initFilePathPrefix(std::string filePath) {
    QVector<std::string> filePathSplit = FileLoader::split(filePath, "/");
    std::string fileName = filePathSplit.at(filePathSplit.size() - 1);
    size_t fileNameLoc = filePath.rfind(fileName);
    filePathPrefix_ = filePath.substr(0, fileNameLoc);
}