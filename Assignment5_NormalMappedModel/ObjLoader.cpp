#include <iostream>
#include <stdexcept>

#include "MtlLoader.h"
#include "ObjLoader.h"

// Global static pointer used to ensure a single instance of the class.
ObjLoader* ObjLoader::loaderInstance_ = NULL;

/**
 * Method for singleton behavior.
 */
ObjLoader* ObjLoader::getInstance() {
    if (!loaderInstance_) {
        loaderInstance_ = new ObjLoader();
    }
    return loaderInstance_;
}

/**
 * Clears the contents of this loader so that it can be used again.
 */
void ObjLoader::clear() {
    FileLoader::clear();
    positions_.clear();
    normals_.clear();
    textureCoordinates_.clear();
    faces_.clear();
    diffuseMapPath_ = "";
}

/**
 * Translates the data of an already loaded .obj file to a format that can be
 * easily used with OpenGL.
 * 
 * @return A pointer to an object containing the translated .obj data for use
 *         with OpenGL.
 * @throws runtimeError if this method is being called before a file has been
 *                   loaded.
 * @throws invalid_argument if an index pair in one of the given faces lies
 *                          outside the bounds of either positions or
 *                          textureCoordinates.
 */
TranslatedObj* ObjLoader::translate() {
    if (!loaded_) {
        throw std::runtime_error("Cannot translate an unloaded .obj file.");
    }
    return TranslatedObj::translate(getPositions(), getTextureCoordinates(), getNormals(), getFaces(), getDiffuseMapPath(), getNormalMapPath());
}

/**
 * Standard private constructor.
 */
ObjLoader::ObjLoader() { }

/**
 * Private copy constructor to enforce singleton behavior.
 */
ObjLoader::ObjLoader(const ObjLoader&) { }

/**
 * Private assignment operator to enforce singleton behavior.
 */
ObjLoader& ObjLoader::operator=(const ObjLoader&) { }

/**
 * Gets the list of vertex position information for this loaded .obj file.
 */
QVector<QVector3D> ObjLoader::getPositions() {
    return positions_;
}

/**
 * Gets the list of vertex normal information for this loaded .obj file.
 */
QVector<QVector3D> ObjLoader::getNormals() {
    return normals_;
}

/**
 * Gets the list of texture coordinates for this loaded .obj file.
 */
QVector<QVector2D> ObjLoader::getTextureCoordinates() {
    return textureCoordinates_;
}

/**
 * Gets the list of face information for this loaded .obj file.
 */
QVector<QVector<QVector3D>> ObjLoader::getFaces() {
    return faces_;
}

/**
 * Gets the diffuse map file path specified in this .obj file's associated
 * .mtl file.
 */
std::string ObjLoader::getDiffuseMapPath() {
    return diffuseMapPath_;
}

/**
 * Gets the normal map file path specified in this .obj file's associated
 * .mtl file.
 */
std::string ObjLoader::getNormalMapPath() {
    return normalMapPath_;
}

/**
 * Takes a face specification line, and if valid, adds the specified face
 * data to the loader's memory.
 *
 * @param splitLine The face specification line, already split into
 *                  different entries.
 * @throws invalid_argument if splitLine does not contain three entries for
 *                          the required number of vertices for a face, or
 *                          if one of the entries contains an imparsable
 *                          vertex index specification.
 */
void ObjLoader::processFaceLine(const QVector<std::string>& splitLine) {
    if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain three entries for "
                                    "the required number of vertices for a "
                                    "face.");
    }
    
    int ii = 1;
    try {
        QVector<QVector3D> face;
        for (; ii < splitLine.size(); ++ii) {
            face.append(ObjLoader::processVertexIndices(splitLine.at(ii)));
        }
        faces_.append(face);
    } catch (std::exception& ex) {
        std::cout << std::endl << ex.what() << std::endl;
        throw std::invalid_argument("The given face specification line "
                                    "contains an imparsable vertex index "
                                    "specification: " + splitLine.at(ii));
    }
}

/**
 * Takes a line, and if valid, adds the corresponding parsed data to the
 * loader's memory.
 *
 * @param line The line to be processed and loaded, if valid.
 * @throws invalid_argument if the line is imparsable as a line of vertex,
 *                          vertex normal, or face data.
 */
void ObjLoader::processLine(const std::string& line) {
    QVector<std::string> splitLine = split(line, ' ');
    try {
        if (splitLine.size() > 0) {
            std::string lineType = splitLine.at(0);
            if (lineType == "v") {
                ObjLoader::processVertexPositionLine(splitLine);
            } else if (lineType == "vn") {
                ObjLoader::processVertexNormalLine(splitLine);
            } else if (lineType == "vt") {
                ObjLoader::processTextureCoordinateLine(splitLine);
            } else if (lineType == "f") {
                ObjLoader::processFaceLine(splitLine);
            } else if (lineType == "mtllib") {
                ObjLoader::processMtllibLine(splitLine);
            }
        }
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        throw std::invalid_argument("The following line is imparsable: \"" + line + "\"");
    }
}

/**
 * Takes a material library specification line, and if valid, loads the
 * .mtl file to be parsed for the file path for the diffuse and normal maps.
 *
 * @param splitLine The material library specification line, already split
 *                  into different entries.
 * @throws invalid_argument if the line does not contain a proper mtllib
 *                          declaration, or if the referenced .mtl file
 *                          cannot be loaded.
 */
void ObjLoader::processMtllibLine(const QVector<std::string>& splitLine) {
    if (splitLine.size() != 2) {
        throw std::invalid_argument("Line does not contain a proper mtllib "
                                    "declaration.");
    }

    std::string mtlLibPath = splitLine.at(1);
    MtlLoader* mtlLoader = MtlLoader::getInstance();
    try {
        mtlLoader->loadFile(filePathPrefix_ + mtlLibPath);
    } catch (std::exception& ex) {
        std::cout << std::endl << ex.what() << std::endl;
        throw std::invalid_argument("The referenced .mtl file cannot be "
                                    "loaded: " + mtlLibPath);
    }
    diffuseMapPath_ = mtlLoader->getDiffuseMapPath();
    normalMapPath_ = mtlLoader->getNormalMapPath();
    mtlLoader->clear();
}

/**
 * Takes a vertex position specification line, and if valid, adds the
 * specified vertex position to the loader's memory.
 *
 * @param splitLine The vertex position specification line, already split
 *                  into different entries.
 * @throws invalid_argument if splitLine does not contain an entry for each
 *                          of the three components of a position vector,
 *                          or if no float conversion could be performed on
 *                          one of its entries.
 * @throws out_of_range if the value read from one of the entries of
 *                      splitLine is out of the range of representable
 *                      values by a float.
 */
void ObjLoader::processVertexPositionLine(const QVector<std::string>& splitLine) {
    if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain an entry for each "
                                    "of the three components of a position "
                                    "vector.");
    }

    std::string xStr = splitLine.at(1);
    std::string yStr = splitLine.at(2);
    std::string zStr = splitLine.at(3);

    float x = stof(xStr);
    float y = stof(yStr);
    float z = stof(zStr);

    QVector3D vertexPosition = QVector3D(x, y, z);
    positions_.append(vertexPosition);
}

/**
 * Takes a vertex normal specification line, and if valid, adds the
 * specified vertex normal to the loader's memory.
 *
 * @param splitLine The vertex normal specification line, already split
 *                  into different entries.
 * @throws invalid_argument if splitLine does not contain an entry for each
 *                          of the three components of a normal vector, or if
 *                          no float conversion could be performed on one of
 *                          its entries.
 * @throws out_of_range if the value read from one of the entries of
 *                      splitLine is out of the range of representable
 *                      values by a float.
 */
void ObjLoader::processVertexNormalLine(const QVector<std::string>& splitLine) {
    if (splitLine.size() != 4) {
        throw std::invalid_argument("Line does not contain an entry for each "
                                    "of the three components of a normal "
                                    "vector.");
    }

    std::string xStr = splitLine.at(1);
    std::string yStr = splitLine.at(2);
    std::string zStr = splitLine.at(3);

    float x = stof(xStr);
    float y = stof(yStr);
    float z = stof(zStr);

    QVector3D vertexNormal = QVector3D(x, y, z);
    normals_.append(vertexNormal);
}

/**
 * Takes a texture coordinate specification line, and if valid, adds the
 * specified texture coordinate to the loader's memory.
 *
 * @param splitLine The texture coordinate specification line, already
 *                  split into different entries.
 * @throws invalid_argument if splitLine does not contain an entry for each of
 *                          the two components of a vertex texture vector, or
 *                          if no float conversion could be performed on one of
 *                          its entries.
 * @throws out_of_range if the value read from one of the entries of splitLine
 *                      is out of the range of representable values by a float.
 */
void ObjLoader::processTextureCoordinateLine(const QVector<std::string>& splitLine) {
    if (splitLine.size() != 3) {
        throw std::invalid_argument("Line does not contain an entry for each "
                                    "of the two components of a vertex texture"
                                    "vector.");
    }
    
    std::string sStr = splitLine.at(1);
    std::string tStr = splitLine.at(2);

    float s = stof(sStr);
    float t = stof(tStr);

    QVector2D textureCoordinatePair = QVector2D(s, t);

    textureCoordinates_.append(textureCoordinatePair);
}

/**
 * Processes a vertex index specification, and if valid, adds the specified
 * indices to the loader's memory.
 * 
 * @param vertexIndices The vertex index specification.
 * @return A vector of integers parsed from vertexIndices representing a triple
 *         of associated vertex, texture, and normal indices.
 * @throws bad_alloc if the function needs to allocate storage and fails.
 * @throws invalid_argument if vertPair does not contain an entry for each
 *                          of an associated vertex, vertex texture, and
 *                          vertex normal index, or if no integer conversion
 *                          could be performed on one of the split segments of
 *                          vertexIndices.
 * @throws out_of_range if one of the parsed values is out of the range of
 *                      representable values by an int.
 */
QVector3D ObjLoader::processVertexIndices(const std::string& vertexIndices) {
    QVector<std::string> splitIndices = FileLoader::split(vertexIndices, '/');

    if (splitIndices.size() != 3) {
        throw std::invalid_argument("Vertex index specification does not "
                                    "contain an entry for each of an "
                                    "associated vertex, vertex texture, and "
                                    "vertex normal index.");
    }
    
    std::string vStr = splitIndices.at(0);
    std::string vtStr = splitIndices.at(1);
    std::string vnStr = splitIndices.at(2);

    int v = stoi(vStr);
    int vt = stoi(vtStr);
    int vn = stoi(vnStr);

    return QVector3D(v, vt, vn);
}
