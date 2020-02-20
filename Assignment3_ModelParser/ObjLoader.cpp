#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility> 
#include <vector>

/**
 * Standard constructor.
 */
ObjLoader::ObjLoader() {
    // vertices_;
    // vertexNormals_;
    // faces_;
}

/**
 * Attempts to process the file with the given path into the loader's memory.
 *
 * @param filePath The path of the file to be loaded.
 * @throws illegal_argument if the given file path contains malformed data and
 *                          cannot be fully loaded.
 */
void ObjLoader::loadFile(std::string filePath) {
    ifstream objFile;
    objFile.open(filePath);
    if (objFile.is_open()) {
        std::string line;

        while (getline(objFile, line)) {
            try {
                ObjLoader::processLine(line);
            } catch (exception& e) {
                std::cout << e.what() << '\n';
                throw illegal_argument("The given file path contains malformed"
                                       " data and cannot be fully loaded.");
            }
        }

        objFile.close();

    } else {
        std::cout << "Unable to open file." << std::endl;
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
 *                          data; if it does not contain a minimum of three
 *                          entries to constitute a face; if one of the "words"
 *                          does not contain exactly one of the expected "//"
 *                          delimiter; or if no integer conversion could be
 *                          performed on one of the split segments of line.
 * @throws out_of_range if one of the parsed values is out of the range of
 *                      representable values by an int.
 */
void ObjLoader::processFaceLine(std::string line) {
    std::vector<std::string> splitLine = split(line, ' ');

    if (splitLine[0] != "f") {
        throw invalid_argument("Line prefix only begins with a \"f,\" and "
                               "contains invalid extra characters.");
    } else if (splitLine.size() < 3) {
        throw invalid_argument("Line does not contain three entries for the "
                               "minimum number of vertices for a face.");
    }
    
    std::vector<std::pair<int, int>> face;
    for (int ii = 1; ii < splitLine.size(); ++ii) {
        face.push_back(processVertexPair(splitLine[ii]));
    }

    faces_.push_back(face);
}

/**
 * Takes a line, and if valid, adds the corresponding parsed data to the
 * loader's memory.
 *
 * @param line The line to be processed and loaded, if valid.
 * @throws bad_alloc if the function needs to allocate storage and fails.
 * @throws invalid_argument if the line only begins with a valid character
 *                          sequence and contains further invalid extra
 *                          characters before the actual data; if it does not
 *                          contain the correct or adequate number of entries
 *                          for its corresponding data type; if one of the
 *                          "words" do not satisfy constraints for that data
 *                          type; or if no number conversion could be performed
 *                          on one of the split tokens.
 * @throws out_of_range if a parsed value is out of the range of representable
 *                      values by the corresponding number type.
 */
void ObjLoader::processLine(std::string line) {
    if (line.length() > 1) {
        switch (line[0]) {
        case 'v':
            if (line.length() > 2) {
                if (line[1] == 'n') {
                    ObjLoader::processVertexNormalLine(line);
                } else {
                    ObjLoader::processVertexLine(line);
                }
            }
            break;
        case 'f':
            ObjLoader::processFaceLine(line);
            break;
        default:
        }
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
    std::vector<std::string> splitLine = split(line, ' ');
    if (splitLine[0] != "v") {
        throw invalid_argument("Line prefix only begins with a \"v,\" and "
                               "contains invalid extra characters.");
    } else if (splitLine.size() != 4) {
        throw invalid_argument("Line does not contain three entries for each "
                               "component of a vertex vector.");
    }

    std::string xStr = splitLine[1];
    std::string yStr = splitLine[2];
    std::string zStr = splitLine[3];

    float x = stof(xStr);
    float y = stof(yStr);
    float z = stof(zStr);

    std::vector<std::string> vertex;
    vertex.push_back(x);
    vertex.push_back(y);
    vertex.push_back(z);

    vertices_.push_back(vertex);
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
    std::vector<std::string> splitLine = split(line, ' ');
    if (splitLine[0] != "vn") {
        throw invalid_argument("Line prefix only begins with \"vn,\" and "
                               "contains invalid extra characters.");
    } else if (splitLine.size() != 4) {
        throw invalid_argument("Line does not contain three entries for each "
                               "component of a normal vector.");
    }
    
    std::string xStr = splitLine[1];
    std::string yStr = splitLine[2];
    std::string zStr = splitLine[3];

    float x = stof(xStr);
    float y = stof(yStr);
    float z = stof(zStr);

    std::vector<std::string> vertexNormal;
    vertex.push_back(x);
    vertex.push_back(y);
    vertex.push_back(z);

    vertexNormals_.push_back(vertexNormal);
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
std::pair<int, int> ObjLoader::processVertexPair(std::string vertPair) {
    std::size_t found = vertPair.find("//");
    if (found == std::string::npos) {
        throw invalid_argument("Passed vertPair does not contain the expected "
                               "\"//\" delimiter.");
    }
    std::size_t rfound = vertPair.rfind("//");
    if (found != rfound) {
        throw invalid_argument("Passed vertPair contains more than one of the "
                               "expected \"//\" delimiter.");
    }

    std::string before = vertPair.substr(0, found);
    std::string after = vertPair.substr(found + 2, vertPair.length());

    int vertexIdx = stoi(before);
    int vertexNormalIdx = stoi(after);

    std::pair<int, int> vertexIdxs(vertexIdx, vertexNormalIdx);
    return vertexIdxs;
}

/**
 * Splits a string into constituent pieces by the given delimiter.
 * 
 * @param line  The string to be split into constituent pieces.
 * @param delim The delimiter to separate out the constituent pieces of line.
 * @return A vector containing the  pieces split apart from line.
 */
std::vector<std::string> ObjLoader::split(std::string line, char delim) {
    std::vector<std::string> splitLine;
    std::stringstream stream(line);
    std::string item;

    while (getline(stream, item, delim)) {
        result.push_back(item);
    }

    return result;
}
