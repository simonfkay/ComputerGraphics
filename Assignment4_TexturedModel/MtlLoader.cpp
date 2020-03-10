#include <fstream>
#include <iostream>
#include <stdexcept>

#include "MtlLoader.h"

// Global static pointer used to ensure a single instance of the class.
MtlLoader* MtlLoader::loaderInstance_ = NULL;

/**
 * Method for singleton behavior.
 */
MtlLoader* MtlLoader::getInstance() {
    if (!loaderInstance_) {
        loaderInstance_ = new MtlLoader();
    }
    return loaderInstance_;
}

/**
 * Clears the contents of this loader so that it can be used again.
 */
void ObjLoader::clear() override {
    FileLoader::clear();
    diffuseMapPath_ = "";
}

/**
 * Gets the path for the diffuse map specified in the loaded .mtl file.
 */
std::string MtlLoader::getDiffuseMapPath() {
    return diffuseMapPath_;
}

/**
 * Standard private constructor.
 */
MtlLoader::MtlLoader() { }

/**
 * Private copy constructor to enforce singleton behavior.
 */
MtlLoader(const MtlLoader& that) { }

/**
 * Private assignment operator to enforce singleton behavior.
 */
MtlLoader& operator=(const MtlLoader& that) { }

/**
 * Takes a line, and if valid, adds the corresponding parsed data to the
 * loader's memory.
 *
 * @param line The line to be processed and loaded, if valid.
 * @throws invalid_argument if the line is imparsarsable as a diffuse map file
 *                          name declaration.
 */
void MtlLoader::processLine(const std::string& line) override {
    QVector<std::string> splitLine = split(line, " ");
    try {
        if (splitLine.size() > 0) {
            std::string lineType = splitLine.at(0);
            if (lineType == "map_Kd") {
                MtlLoader::processMapKdLine(splitLine);
            }
        }
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        throw std::invalid_argument("The following line is imparsable: \"" +
                                    line + "\"");
    }
}

/**
 * Takes a diffuse map file name declaration line, and if valid, stores the
 * file path in the loader's memory.
 *
 * @param splitLine The diffuse map file name declaration line, already split
 *                  into different entries.
 * @throws invalid_argument if splitLine does not contain a single additional
 *                          entry past "mtllib" representing the file name; if
 *                          the specified file name is not a .ppm file; or if
 *                          the file at the given path simply cannot be opened.
 */
void MtlLoader::processMapKdLine(const QVector<std::string>& splitLine) {
    if (splitLine.size() != 2) {
        throw std::invalid_argument("Line does not contain a proper diffuse "
                                    "map file name declaration.");
    }

    std::string diffuseMapPath = splitLine.at(1);
    int pathStringLength = diffuseMapPath.size();
    if (pathStringLength < 4 ||
        diffuseMapPath.substr(pathStringLength - 4, pathStringLength) != ".ppm") {
        throw std::invalid_argument("The specified diffuse map file name must "
                                    "be a .ppm file.");
    }

    std::ifstream ppmFile;
    ppmFile.open(filePathPrefix_ + filePath);
    if (ppmFile.is_open()) {
        diffuseMapPath_ = filePathPrefix_ + filePath;
        ppmFile.close();
    } else {
        throw std::invalid_argument("Unable to open file at: " + filePath);
    }
}
