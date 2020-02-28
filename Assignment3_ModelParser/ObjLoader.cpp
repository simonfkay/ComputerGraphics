#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <QPair>
#include <QVector>
#include <QVector3D>

#include "ObjLoader.h"

/**
 * Standard constructor.
 */
ObjLoader::ObjLoader() { }

/**
 * Resets the currently held data in the loader to allow the loading of another
 * .obj file.
 */
void ObjLoader::clear() {
    vertices_ = QVector<QVector3D>();
    vertexNormals_ = QVector<QVector3D>();
    faces_ = QVector<QVector<QPair<int, int>>>();
}

/**
 * Gets an array of vertex coordinates and their matching normals, interleaved
 * for use in OpenGL.
 *
 * @return the vertex information for this loaded .obj file.
 * @throws range_error if the number of vertices and vertex normals are not the
 *                     same.
 */
float* ObjLoader::getVertices(int &numAttr) {
    int numEntries = vertices_.size();

    if (numEntries != vertexNormals_.size()) {
        throw std::range_error("The number of vertices and vertex normals must be the same.");
    }

    int vertexSize = 3 + 3;
    float* vertexInfo = new float[numEntries * vertexSize];

    for (int ii = 0; ii < numEntries; ++ii) {
        vertexInfo[ii * vertexSize + 0] = vertices_.at(ii).x();
        vertexInfo[ii * vertexSize + 1] = vertices_.at(ii).y();
        vertexInfo[ii * vertexSize + 2] = vertices_.at(ii).z();
        vertexInfo[ii * vertexSize + 3] = vertexNormals_.at(ii).x();
        vertexInfo[ii * vertexSize + 4] = vertexNormals_.at(ii).y();
        vertexInfo[ii * vertexSize + 5] = vertexNormals_.at(ii).z();
    }

    numAttr = numEntries * vertexSize;
    return vertexInfo;
}

/**
 * Gets an array of vertex indices.
 *
 * @return the vertex index information for this loaded .obj file.
 * @throws range_error if an entry refers to an index that is greater than the
 *                     size of vertices_.
 */
int* ObjLoader::getIndices(int &numAttr) {
    QVector<int> indices;
    for (QVector<QPair<int, int>> face : faces_) {
        for (QPair<int, int> vertexPair : face) {
            int index = vertexPair.first - 1;

            if (index >= vertices_.size()) {
                throw std::range_error("Vertex index list cannot reference a vertex without that index.");
            }

            indices << index;
        }
    }
    int* indexArray = new int[indices.size()];
    for (int ii = 0; ii < indices.size(); ++ii) {
        indexArray[ii] = indices[ii];
    }

    numAttr = indices.size();
    return indexArray;
}

/**
 * Attempts to process the file with the given path into the loader's memory.
 *
 * @param filePath The path of the file to be loaded.
 * @throws invalid_argument if the given file path contains malformed data and
 *                          cannot be fully loaded.
 */
void ObjLoader::loadFile(std::string filePath) {
    std::ifstream objFile;
    objFile.open(filePath);
    if (objFile.is_open()) {
        std::string line;

        while (getline(objFile, line)) {
            try {
                ObjLoader::processLine(line);
            } catch (std::exception& ex) {
                std::cout << std::endl << ex.what() << std::endl;
                throw std::invalid_argument("The given file path contains malformed"
                                            " data and cannot be fully loaded.");
            }
        }

        objFile.close();
    } else {
        std::cout << "Unable to open file at: " << filePath << std::endl;
    }
}

/**
 * Takes a line beginning with "f," and if valid, adds the specified face
 * to the loader's memory.
 *
 * @param line The line to be verified and added as vertex data.
 * @throws bad_alloc if the function needs to allocate storage and fails.
 * @throws invalid_argument if the line only begins with "f" and contains
 *                          further invalid extra characters before the actual
 *                          data; if it does not contain three entries to
 *                          constitute a face; if one of the "words" does not
 *                          contain exactly one of the expected "//" delimiter;
 *                          or if no integer conversion could be performed on
 *                          one of the split segments of line.
 * @throws out_of_range if one of the parsed values is out of the range of
 *                      representable values by an int.
 */
void ObjLoader::processFaceLine(std::string line) {
    QVector<std::string> splitLine = split(line, ' ');

    if (splitLine[0] != "f") {
        throw std::invalid_argument("Line prefix only begins with a \"f,\" and "
                                    "contains invalid extra characters.");
    } else if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain three entries for the "
                                    "required number of vertices for a face.");
    }
    
    QVector<QPair<int, int>> face;
    for (int ii = 1; ii < splitLine.size(); ++ii) {
        face << processVertexPair(splitLine[ii]);
    }

    faces_ << face;
}

/**
 * Takes a line, and if valid, adds the corresponding parsed data to the
 * loader's memory.
 *
 * @param line The line to be processed and loaded, if valid.
 * @throws invalid_argument if the line is imparsable as a line of vertex,
 *                          vertex normal, or face data.
 */
void ObjLoader::processLine(std::string line) {
    try {
        if (line.length() > 1) {
            switch (line[0]) {
            case 'v':
                if (line.length() > 2) {
                    switch (line[1]) {
                    case 'n':
                        ObjLoader::processVertexNormalLine(line);
                        break;
                    case 't':
                        // Vertex texture lines should be ignored, for now
                        return;
                    default:
                        ObjLoader::processVertexLine(line);
                    }
                }
                break;
            case 'f':
                ObjLoader::processFaceLine(line);
                break;
            }
        }
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        throw std::invalid_argument("The following line is imparsable: \"" + line + "\"");
    }
}

/**
 * Takes a line beginning with "v," and if valid, adds the specified vertex
 * to the loader's memory.
 *
 * @param line The line to be verified and added as vertex data.
 * @throws invalid_argument if the line only begins with "v" and contains
 *                          further invalid extra characters before the actual
 *                          data; if it does not contain an entry for each of
 *                          the three vertex normal components; or if no float
 *                          conversion could be performed on one of the split
 *                          tokens.
 * @throws out_of_range if the value read from one of the split tokens is out
 *                      of the range of representable values by a float.
 */
void ObjLoader::processVertexLine(std::string line) {
    QVector<std::string> splitLine = split(line, ' ');
    if (splitLine[0] != "v") {
        throw std::invalid_argument("Line prefix only begins with a \"v,\" and "
                                    "contains invalid extra characters.");
    } else if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain three entries for each "
                                    "component of a vertex vector.");
    }

    std::string xStr = splitLine[1];
    std::string yStr = splitLine[2];
    std::string zStr = splitLine[3];

    float x = stof(xStr);
    float y = stof(yStr);
    float z = stof(zStr);

    QVector3D vertex = QVector3D(x, y, z);
    vertices_ << vertex;
}

/**
 * Takes a line beginning with "vn," and if valid, adds the specified vertex
 * normal to the loader's memory.
 *
 * @param line The line to be verified and added as vertex normal data.
 * @throws invalid_argument if the line only begins with "vn" and contains
 *                          further invalid extra characters before the actual
 *                          data; if it does not contain three entries for each
 *                          vertex normal component; or if no float conversion
 *                          could be performed on one of the split tokens.
 * @throws out_of_range if the value read from one of the split tokens is out
 *                      of the range of representable values by a float.
 */
void ObjLoader::processVertexNormalLine(std::string line) {

    // std::cout << line << std::endl;
    QVector<std::string> splitLine = split(line, ' ');
    if (splitLine[0] != "vn") {
        throw std::invalid_argument("Line prefix only begins with \"vn,\" and "
                                    "contains invalid extra characters.");
    } else if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain three entries for each "
                                    "component of a normal vector.");
    }
    
    std::string xStr = splitLine[1];
    std::string yStr = splitLine[2];
    std::string zStr = splitLine[3];

    float x = stof(xStr);
    float y = stof(yStr);
    float z = stof(zStr);

    QVector3D vertexNormal = QVector3D(x, y, z);

    vertexNormals_ << vertexNormal;
}

/**
 * Processes a string with the assumption that it has been parsed out as a
 * "word" on a face line in the .obj file.
 * 
 * @param vertPair A string representing the indices of a vertex and its
 *                 corresponding vertex normal.
 * @return A pair of integers parsed from each end of the original string.
 * @throws bad_alloc if the function needs to allocate storage and fails.
 * @throws invalid_argument if vertPair does not contain exactly one of the
 *                          expected "//" delimiter; or if no integer
 *                          conversion could be performed on one of the split
 *                          segments of line.
 * @throws out_of_range if one of the parsed values is out of the range of
 *                      representable values by an int.
 */
QPair<int, int> ObjLoader::processVertexPair(std::string vertPair) {
    std::size_t found = vertPair.find("//");
    if (found == std::string::npos) {
        throw std::invalid_argument("Passed vertPair does not contain the expected "
                                    "\"//\" delimiter.");
    }
    std::size_t rfound = vertPair.rfind("//");
    if (found != rfound) {
        throw std::invalid_argument("Passed vertPair contains more than one of the "
                                    "expected \"//\" delimiter.");
    }

    std::string before = vertPair.substr(0, found);
    std::string after = vertPair.substr(found + 2, vertPair.length());

    int vertexIdx = stoi(before);
    int vertexNormalIdx = stoi(after);

    QPair<int, int> vertexIdxs = QPair<int, int>(vertexIdx, vertexNormalIdx);
    return vertexIdxs;
}

/**
 * Splits a string into constituent pieces by the given delimiter.
 * 
 * @param line  The string to be split into constituent pieces.
 * @param delim The delimiter to separate out the constituent pieces of line.
 * @return A vector containing the  pieces split apart from line.
 */
QVector<std::string> ObjLoader::split(std::string line, char delim) {
    QVector<std::string> splitLine;
    std::stringstream stream(line);
    std::string item;

    while (getline(stream, item, delim)) {
        splitLine << item;
    }

    return splitLine;
}
