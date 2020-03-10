#pragma once

#include <QPair>
#include <QVector2D>
#include <QVector3D>

#include "FileLoader.h"

/**
 * Class to load .obj files.
 */
class ObjLoader : public FileLoader {
public:
    /**
     * Standard constructor.
     */
    ObjLoader();

    /**
     * Gets the list of index information for this loaded .obj file.
     */
    QVector<unsigned int> getIndices();

    /**
     * Gets the list of vertex information for this loaded .obj file.
     */
    QVector<QVector3D> getVertices();

    /**
     * Gets the list of vertex texture information for this loaded .obj file.
     */
    QVector<QVector2D> getVertexTextures();

private:
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
    void processFaceLine(QVector<std::string> splitLine);

    /**
     * Takes a line, and if valid, adds the corresponding parsed data to the
     * loader's memory.
     *
     * @param line The line to be processed and loaded, if valid.
     * @throws invalid_argument if the line is imparsable as a line of vertex,
     *                          vertex normal, or face data.
     */
    void processLine(std::string line);

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
    void processMtllibLine(QVector<std::string> splitLine);

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
    void processVertexLine(QVector<std::string> splitLine);

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
    void processVertexTextureLine(QVector<std::string> splitLine);

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
    QPair<int, int> processVertexIndices(std::string vertexIndices);

    // Loaded data:
    QVector<QVector3D> vertices_;
    QVector<QVector2D> vertexTextures_;
    QVector<QVector<QPair<int, int>>> faces_;
    std::string diffuseMapPath_;
};
