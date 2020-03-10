#pragma once

#include <QPair>
#include <QVector2D>
#include <QVector3D>

#include "FileLoader.h"
#include "TranslatedObj.h"

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
     * Translates the data of an already loaded .obj file to a format that can
     * be easily used with OpenGL.
     * 
     * @return A pointer to an object containing the translated .obj data for
     *         use with OpenGL.
     * @throws exception if this method is being called before a file has been
     *                   loaded.
     * @throws invalid_argument if an index pair in one of the given faces lies
     *                          outside the bounds of either positions or
     *                          textureCoordinates.
     */
    TranslatedObj* translate();

private:
    /**
     * Gets the list of vertex position information for this loaded .obj file.
     */
    QVector<QVector3D> getPositions();

    /**
     * Gets the list of texture coordinates for this loaded .obj file.
     */
    QVector<QVector2D> getTextureCoordinates();

    /**
     * Gets the list of face information for this loaded .obj file.
     */
    QVector<QVector<QPair<int, int>>> getFaces();

    /**
     * Gets the diffuse map file path specified in this .obj file's associated
     * .mtl file.
     */
    std::string getDiffuseMapPath();

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
    void processFaceLine(const QVector<std::string>& splitLine);

    /**
     * Takes a line, and if valid, adds the corresponding parsed data to the
     * loader's memory.
     *
     * @param line The line to be processed and loaded, if valid.
     * @throws invalid_argument if the line is imparsable as a line of vertex,
     *                          vertex normal, or face data.
     */
    void processLine(const std::string& line) override;

    /**
     * Takes a material library specification line, and if valid, loads the
     * .mtl file to be parsed for the file path for the diffuse map.
     *
     * @param splitLine The material library specification line, already split
     *                  into different entries.
     * @throws invalid_argument if the line does not contain a proper mtllib
     *                          declaration, or if the referenced .mtl file
     *                          cannot be loaded.
     */
    void processMtllibLine(const QVector<std::string>& splitLine);

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
    void processVertexPositionLine(const QVector<std::string>& splitLine);

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
    void processTextureCoordinateLine(const QVector<std::string>& splitLine);

    /**
     * Processes a vertex index specification, and if valid, adds the specified
     * indices to the loader's memory.
     * 
     * @param vertexIndices The vertex index specification.
     * @return A pair of integers parsed from vertexIndices representing a pair
     *         of associated vertex and texture indices.
     * @throws bad_alloc if the function needs to allocate storage and fails.
     * @throws invalid_argument if vertPair does not contain an entry for each
     *                          of an associated vertex, vertex texture, and
     *                          vertex normal index (the last of which can be
     *                          empty), or if no integer conversion could be
     *                          performed on one of the split segments of
     *                          vertexIndices.
     * @throws out_of_range if one of the parsed values is out of the range of
     *                      representable values by an int.
     */
    static QPair<int, int> processVertexIndices(const std::string& vertexIndices);

    // Loaded data:
    QVector<QVector3D> positions_;
    QVector<QVector2D> textureCoordinates_;
    QVector<QVector<QPair<int, int>>> faces_;
    std::string diffuseMapPath_;
};
