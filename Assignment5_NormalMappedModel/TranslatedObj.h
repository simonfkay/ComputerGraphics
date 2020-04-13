#pragma once

#include <QPair>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include "IndexedVertex.h"

/**
 * Represents an already-loaded .obj file that has been translated into a
 * format for easy OpenGL access.
 */
class TranslatedObj {
public:
    /**
     * Standard destructor.
     */
    ~TranslatedObj();

    /**
     * Gets the vertex data for this translated .obj file to be fed to OpenGL.
     */
    float* getData();

    /**
     * Gets the index data for this translated .obj file to be fed to OpenGL.
     */
    unsigned int* getIndices();

    /**
     * Gets the number of entries in data_.
     */
    unsigned int getNumData();

    /**
     * Gets the number of entries in indices_.
     */
    unsigned int getNumIndices();

    /**
     * Gets the size of an individual vertex (i.e. how many floats each one
     * contains).
     */
    unsigned int getVertexSize();

    /**
     * Gets the diffuse map file path for this translated .obj file to be fed to
     * OpenGL.
     */
    std::string getDiffuseMapPath();

    /**
     * Gets the normal map file path for this translated .obj file to be fed to
     * OpenGL.
     */
    std::string getNormalMapPath();

    /**
     * Translates the data of an already loaded .obj file to a format that can
     * be easily used with OpenGL.
     * 
     * @param positions The list of distinct vertex positions for the object
     *                  model.
     * @param textureCoordinates The list of distinct texture coordinates for
     *                           the object model.
     * @param normals The list of distinct vertex normals for the object model.
     * @param faces The list of face data, which comprises of ordered triplets
     *              of index triplets, the first corresponding to the positions
     *              list, the second corresponding to to the textureCoordinates
     *              list, and the third corresponding to the normals list.
     * @param diffuseMapPath The file path to the corresponding diffuse map for
     *                       object texturing.
     * @return A pointer to an object containing the translated .obj data for
     *         use with OpenGL.
     * @throws invalid_argument if an index pair in one of the given faces lies
     *                          outside the bounds of either positions or
     *                          textureCoordinates.
     */
    static TranslatedObj* translate(const QVector<QVector3D>& positions,
                                    const QVector<QVector2D>& textureCoordinates,
                                    const QVector<QVector3D>& normals,
                                    const QVector<QVector<QVector3D>>& faces,
                                    const std::string& diffuseMapPath,
                                    const std::string& normalMapPath);

private:
    /**
     * Standard parametrized private constructor for translated object data.
     */
    TranslatedObj(float* data,
                  unsigned int* indices,
                  unsigned int numData,
                  unsigned int numIndices,
                  unsigned int vertexSize,
                  const std::string& diffuseMapPath,
                  const std::string& normalMapPath);

    /**
     * Reorders vertex data for use in OpenGL.
     * 
     * @param positions The list of distinct vertex positions for the object
     *                  model.
     * @param textureCoordinates The list of distinct texture coordinates for
     *                           the object model.
     * @param normals The list of distinct vertex normals for the object model.
     * @param faces The list of face data, which comprises of ordered triplets
     *              of index triplets, the first corresponding to the positions
     *              list, the second corresponding to to the textureCoordinates
     *              list, and the third corresponding to the normals list.
     * @return A pair of lists where the first is a list of the reordered
     *         vertex data, grouping together associated position, texture
     *         coordinate, and normal values, and the second is a list of these
     *         newly indexed vertices in the same order as specified in faces.
     * @throws invalid_argument if an index pair in one of the given faces lies
     *                          outside the bounds of either positions,
     *                          textureCoordinates, or normals.
     */
    static QPair<QVector<IndexedVertex*>, QVector<unsigned int>> reorderVertexData(const QVector<QVector3D>& positions,
                                                                                   const QVector<QVector2D>& textureCoordinates,
                                                                                   const QVector<QVector3D>& normals,
                                                                                   const QVector<QVector<QVector3D>>& faces);

    // Vertex and face data
    float* data_;
    unsigned int* indices_;

    // Constants to be used later in OpenGL processing
    unsigned int numData_;
    unsigned int numIndices_;
    unsigned int vertexSize_;

    // Other OpenGL data
    std::string diffuseMapPath_;
    std::string normalMapPath_;
};