#pragma once

#include <string>

#include <QPair>
#include <QVector>
#include <QVector3D>

/**
 * Class to load .obj files.
 */
class ObjLoader {
public:
    /**
     * Standard constructor.
     */
    ObjLoader();

    /**
     * Resets the currently held data in the loader to allow the loading of another
     * .obj file.
     */
    void clear();

    /**
     * Gets an array of vertex coordinates and their matching normals, interleaved
     * for use in OpenGL.
     *
     * @return the vertex information for this loaded .obj file.
     * @throws range_error if the number of vertices and vertex normals are not the
     *                     same.
     */
    float* getVertices(int &numAttr);

    /**
     * Gets an array of vertex indices.
     *
     * @return the vertex index information for this loaded .obj file.
     * @throws range_error if an entry refers to an index that is greater than the
     *                     size of vertices_.
     */
    int* getIndices(int &numAttr);

    /**
     * Attempts to process the file with the given path into the loader's memory.
     *
     * @param filePath The path of the file to be loaded.
     * @throws illegal_argument if the given file path contains malformed data and
     *                          cannot be fully loaded.
     */
    void loadFile(std::string filePath);

private:
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
    void processFaceLine(std::string line);

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
    void processVertexLine(std::string line);

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
    void processVertexNormalLine(std::string line);

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
    QPair<int, int> processVertexPair(std::string vertPair);

    /**
     * Splits a string into constituent pieces by the given delimiter.
     * 
     * @param line  The string to be split into constituent pieces.
     * @param delim The delimiter to separate out the constituent pieces of line.
     * @return A vector containing the  pieces split apart from line.
     */
    QVector<std::string> split(std::string line, char delim);

    // Loaded data:
    QVector<QVector3D> vertices_;
    QVector<QVector3D> vertexNormals_;
    QVector<QVector<QPair<int, int>>> faces_;
};
