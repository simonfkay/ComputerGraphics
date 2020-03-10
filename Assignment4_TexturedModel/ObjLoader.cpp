#include <stdexcept>

#include "MtlLoader.h"
#include "ObjLoader.h"

/**
 * Standard constructor.
 */
ObjLoader::ObjLoader() { }

/**
 * Gets the list of index information for this loaded .obj file.
 */
QVector<unsigned int> ObjLoader::getIndices() {
    QVector<unsigned int> indices = QVector<unsigned int>();
    for (QVector<QPair<int, int>> face : faces_) {
        for (int ii = 0; ii < face.size(); ++ii) {
            QPair<int, int> indexPair = face.at(ii);
            unsigned int index = indexPair.first;
            indices << index;
        }
    }
    return indices;
}

/**
 * Gets the list of vertex information for this loaded .obj file.
 */
QVector<QVector3D> ObjLoader::getVertices() {
    return vertices_;
}

/**
 * Gets the list of vertex texture information for this loaded .obj file.
 */
QVector<QVector2D> ObjLoader::getVertexTextures() {
    return vertexTextures_;
}

/**
 * Takes face specification line, and if valid, adds the specified face data to
 * the loader's memory.
 *
 * @param splitLine The face specification line, already split into different
 *                  entries.
 * @throws invalid_argument if splitLine does not contain three entries for the
 *                          required number of vertices for a face, or if one
 *                          of the entries contains an imparsable vertex index
 *                          specification.
 */
void ObjLoader::processFaceLine(QVector<std::string> splitLine) {
    if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain three entries for the "
                                    "required number of vertices for a face.");
    }
    
    int ii = 1;
    try {
        QVector<QPair<int, int>> face;
        for (; ii < splitLine.size(); ++ii) {
            face << processVertexPair(splitLine[ii]);
        }
        faces_ << face;
    } catch (std::exception& ex) {
        std::cout << std::endl << ex.what() << std::endl;
        throw std::invalid_argument("The given face specification line "
                                    "contains an imparsable vertex index "
                                    "specification: " + splitLine[ii]);
    }
}

/**
 * Takes a line, and if valid, adds the corresponding parsed data to the
 * loader's memory.
 *
 * @param line The line to be processed and loaded, if valid.
 * @throws invalid_argument if the line is imparsable as a line of vertex,
 *                          vertex texture, material file, or face data.
 */
void ObjLoader::processLine(std::string line) override {
    QVector<std::string> splitLine = split(line, " ");
    try {
        if (splitLine.size() > 0) {
            std::string lineType = splitLine[0];
            if (lineType == "v") {
                ObjLoader::processVertexLine(splitLine);
            } else if (lineType == "vt") {
                ObjLoader::processVertexTextureLine(splitLine);
            } else if (lineType == "f") {
                ObjLoader::processFaceLine(splitLine);
            } else if (lineType == "mtllib") {
                ObjLoader::processMtllibLine(splitLine);
            } else {
                // Ignore any irrellevant lines
            }
        }
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        throw std::invalid_argument("The following line is imparsable: \"" + line + "\"");
    }
}

/**
 * Takes a material library specification line, and if valid, loads the .mtl
 * file to be parsed for the file path for the diffuse map.
 *
 * @param splitLine The material library specification line, already split into
 *                  different entries.
 * @throws invalid_argument if the line does not contain a proper mtllib
 *                          declaration, or if the referenced .mtl file cannot
 *                          be loaded.
 */
void ObjLoader::processMtllibLine(QVector<std::string> splitLine) {
    if (splitLine.size() != 2) {
        throw std::invalid_argument("Line does not contain a proper mtllib "
                                    "declaration.");
    }

    std::string mtlLibPath = splitLine[1];
    MtlLoader mtlLoader = MtlLoader();
    try {
        mtlLoader.loadFile(filePathPrefix_ + mtlLibPath);
    } catch (std::exception& ex) {
        std::cout << std::endl << ex.what() << std::endl;
        throw std::invalid_argument("The referenced .mtl file cannot be loaded: " mtlLibPath);
    }
    diffuseMapPath_ = mtlLoader.getDiffuseMapPath();
}

/**
 * Takes vertex specification line, and if valid, adds the specified vertex
 * to the loader's memory.
 *
 * @param splitLine The vertex specification line, already split into different
 *                  entries.
 * @throws invalid_argument if splitLine does not contain an entry for each of
 *                          the three components of a vertex vector, or if no
 *                          float conversion could be performed on one of its
 *                          entries.
 * @throws out_of_range if the value read from one of the entries of splitLine
 *                      is out of the range of representable values by a float.
 */
void ObjLoader::processVertexLine(QVector<std::string> splitLine) {
    if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain an entry for each "
                                    "of the three components of a vertex "
                                    "vector.");
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
 * Takes vertex texture specification line, and if valid, adds the specified
 * vertex texture vector to the loader's memory.
 *
 * @param splitLine The vertex texture specification line, already split into
 *                  different entries.
 * @throws invalid_argument if splitLine does not contain an entry for each of
 *                          the two components of a vertex texture vector, or
 *                          if no float conversion could be performed on one of
 *                          its entries.
 * @throws out_of_range if the value read from one of the entries of splitLine
 *                      is out of the range of representable values by a float.
 */
void ObjLoader::processVertexTextureLine(QVector<std::string> splitLine) {
    if (splitLine.size() != 3) {
        throw std::invalid_argument("Line does not contain an entry for each "
                                    "of the two components of a vertex texture"
                                    "vector.");
    }
    
    std::string sStr = splitLine[1];
    std::string tStr = splitLine[2];

    float s = stof(sStr);
    float t = stof(tStr);

    QVector2D vertexTexture = QVector2D(s, t);

    vertexTextures_ << vertexTexture;
}

/**
 * Processes a vertex index specification, and if valid, adds the specified
 * indices to the loader's memory.
 * 
 * @param vertexIndices The vertex index specification.
 * @return A pair of integers parsed from vertexIndices representing a pair of
 *         associated vertex and texture indices.
 * @throws bad_alloc if the function needs to allocate storage and fails.
 * @throws invalid_argument if vertPair does not contain an entry for each of
 *                          an associated vertex, vertex texture, and vertex
 *                          normal index (the last of which can be empty), or
 *                          if no integer conversion could be performed on one
 *                          of the split segments of vertexIndices.
 * @throws out_of_range if one of the parsed values is out of the range of
 *                      representable values by an int.
 */
QPair<int, int> ObjLoader::processVertexIndices(std::string vertexIndices) {
    QVector<std::string> splitIndices = split(vertexIndices, "/");

    if (splitIndices.size() != 3) {
        throw std::invalid_argument("Vertex index specification does not "
                                    "contain an entry for each of an "
                                    "associated vertex, vertex texture, and "
                                    "vertex normal index (the last of which "
                                    "can be empty).");
    
    std::string vStr = splitIndices[0];
    std::string vtStr = splitIndices[1];

    float v = stoi(vStr);
    float vt = stoi(vtStr);

    QPair<int, int> vertexIdxs = QPair<int, int>(s, t);
    return vertexIdxs;
}
