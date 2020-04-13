#include <map>

#include "TranslatedObj.h"

/**
 * Standard destructor.
 */
TranslatedObj::~TranslatedObj() {
    delete[] data_;
    delete[] indices_;
}

/**
 * Gets the vertex data for this translated .obj file to be fed to OpenGL.
 */
float* TranslatedObj::getData() {
    return data_;
}

/**
 * Gets the index data for this translated .obj file to be fed to OpenGL.
 */
unsigned int* TranslatedObj::getIndices() {
    return indices_;
}

/**
 * Gets the number of entries in data_.
 */
unsigned int TranslatedObj::getNumData() {
    return numData_;
}

/**
 * Gets the number of entries in indices_.
 */
unsigned int TranslatedObj::getNumIndices() {
    return numIndices_;
}

/**
 * Gets the size of an individual vertex (i.e. how many floats each one
 * contains).
 */
unsigned int TranslatedObj::getVertexSize() {
    return vertexSize_;
}

/**
 * Gets the diffuse map file path for this translated .obj file to be fed to
 * OpenGL.
 */
std::string TranslatedObj::getDiffuseMapPath() {
    return diffuseMapPath_;
}

/**
 * Gets the normal map file path for this translated .obj file to be fed to
 * OpenGL.
 */
std::string TranslatedObj::getNormalMapPath() {
    return normalMapPath_;
}

// [TODO: Inside]
/**
 * Translates the data of an already loaded .obj file to a format that can be
 * easily used with OpenGL.
 * 
 * @param positions The list of distinct vertex positions for the object model.
 * @param normals The list of distinct vertex normals for the object model.
 * @param textureCoordinates The list of distinct texture coordinates for the
 *                           object model.
 * @param faces The list of face data, which comprises of ordered triplets of
 *              index triplets, the first corresponding to the positions list,
 *              the second corresponding to to the textureCoordinates list, and
 *              the third corresponding to the normals list.
 * @param diffuseMapPath The file path to the corresponding diffuse map for
 *                       object texturing.
 * @param normalMapPath The file path to the corresponding normal map for
 *                       object texturing.
 * @return A pointer to an object containing the translated .obj data for use
 *         with OpenGL.
 * @throws invalid_argument if an index pair in one of the given faces lies
 *                          outside the bounds of either positions or
 *                          textureCoordinates.
 */
TranslatedObj* TranslatedObj::translate(const QVector<QVector3D>& positions, const QVector<QVector2D>& textureCoordinates, const QVector<QVector3D>& normals, const QVector<QVector<QVector3D>>& faces, const std::string& diffuseMapPath, const std::string& normalMapPath) {
    // Reorder vertex data to make sense for OpenGL
    QPair<QVector<IndexedVertex*>, QVector<unsigned int>> reorderedVertexData = TranslatedObj::reorderVertexData(positions, textureCoordinates, normals, faces);
    QVector<IndexedVertex*> indexedVertices = reorderedVertexData.first;

    QVector<unsigned int> faceIndices = reorderedVertexData.second;

    // Initialize constants
    unsigned int numIndexedVertices = indexedVertices.size();
    unsigned int vertexSize = 3 + 2 + 3 + 3; // x, y, z; s, t; xn, yn, zn; xt, yt, zt;
    unsigned int numData = numIndexedVertices * vertexSize;
    unsigned int numIndices = faceIndices.size();

    // Calculate tangents
    for (unsigned int ii = 0; ii < numIndices; ii += 3) {
        IndexedVertex* v0 = indexedVertices.at(faceIndices.at(ii));
        IndexedVertex* v1 = indexedVertices.at(faceIndices.at(ii + 1));
        IndexedVertex* v2 = indexedVertices.at(faceIndices.at(ii + 2));

        QVector3D tangent;

        TranslatedObj::computeTangentBasis(v0, v1, v2, tangent);
    }

    // Convert reordered vertex data to array
    float* data = new float[numData];
    for (unsigned int ii = 0; ii < numIndexedVertices; ++ii) {
        IndexedVertex* nextVertex = indexedVertices.at(ii);
        QVector3D pos = nextVertex->position_;
        QVector2D uv = nextVertex->textureCoordinates_;
        QVector3D norm = nextVertex->normal_;
        QVector3D tangent = nextVertex->getFinalTangent();

        data[ii * vertexSize + 0] = pos.x();
        data[ii * vertexSize + 1] = pos.y();
        data[ii * vertexSize + 2] = pos.z();

        data[ii * vertexSize + 3] = uv.x();
        data[ii * vertexSize + 4] = uv.y();

        data[ii * vertexSize + 5] = norm.x();
        data[ii * vertexSize + 6] = norm.y();
        data[ii * vertexSize + 7] = norm.z();

        data[ii * vertexSize + 8] = tangent.x();
        data[ii * vertexSize + 9] = tangent.y();
        data[ii * vertexSize + 10] = tangent.z();
    }

    unsigned int* indices = new unsigned int[numIndices];
    for (unsigned int ii = 0; ii < numIndices; ++ii) {
        indices[ii] = faceIndices.at(ii);
    }

    TranslatedObj* translated = new TranslatedObj(data, indices, numData, numIndices, vertexSize, diffuseMapPath, normalMapPath);

    for (IndexedVertex* indexedVertex : indexedVertices) {
        delete indexedVertex;
    }

    return translated;
}

/**
 * Standard parametrized private constructor for translated object data.
 */
TranslatedObj::TranslatedObj(float* data,
                             unsigned int* indices,
                             unsigned int numData,
                             unsigned int numIndices,
                             unsigned int vertexSize,
                             const std::string& diffuseMapPath,
                             const std::string& normalMapPath) : data_(data),
                                                                 indices_(indices),
                                                                 numData_(numData),
                                                                 numIndices_(numIndices),
                                                                 vertexSize_(vertexSize),
                                                                 diffuseMapPath_(diffuseMapPath),
                                                                 normalMapPath_(normalMapPath) { }

// [TODO: Write comment, complete function
void TranslatedObj::computeTangentBasis(IndexedVertex* v0, IndexedVertex* v1, IndexedVertex* v2, QVector3D& tangent) {
    // Shortcuts for vertex positions
    QVector3D& pos0 = v0->position_;
    QVector3D& pos1 = v1->position_;
    QVector3D& pos2 = v2->position_;

    // Shortcuts for tex coordinates
    QVector2D& uv0 = v0->textureCoordinates_;
    QVector2D& uv1 = v1->textureCoordinates_;
    QVector2D& uv2 = v2->textureCoordinates_;

    // Position deltas
    QVector3D deltaPos1 = pos1 - pos0;
    QVector3D deltaPos2 = pos2 - pos0;

    // UV deltas
    QVector2D deltaUV1 = uv1 - uv0;
    QVector2D deltaUV2 = uv2 - uv0;

    // Calculate tangent
    float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
    tangent = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y()) * r;

    // Set tangent data in vertex information
    v0->addTangent(tangent);
    v1->addTangent(tangent);
    v2->addTangent(tangent);
}

/**
 * Reorders vertex data for use in OpenGL.
 * 
 * @param positions The list of distinct vertex positions for the object model.
 * @param textureCoordinates The list of distinct texture coordinates for the
 *                           object model.
 * @param normals The list of distinct vertex normals for the object model.
 * @param faces The list of face data, which comprises of ordered triplets of
 *              index triplets, the first corresponding to the positions list,
 *              the second corresponding to to the textureCoordinates list, and
 *              the third corresponding to the normals list.
 * @return A pair of lists where the first is a list of the reordered vertex
 *         data, grouping together associated position, texture coordinate, and
 *         normal values, and the second is a list of these newly indexed
 *         vertices in the same order as specified in faces.
 * @throws invalid_argument if an index pair in one of the given faces lies
 *                          outside the bounds of either positions,
 *                          textureCoordinates, or normals.
 */
QPair<QVector<IndexedVertex*>, QVector<unsigned int>> TranslatedObj::reorderVertexData(const QVector<QVector3D>& positions, const QVector<QVector2D>& textureCoordinates, const QVector<QVector3D>& normals, const QVector<QVector<QVector3D>>& faces) {
    std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, IndexedVertex*>>> quickLookup;
    QVector<IndexedVertex*> newOrdering;
    QVector<unsigned int> faceIndices;

    for (unsigned int ii = 0; ii < faces.size(); ++ii) {
        QVector<QVector3D> face = faces.at(ii);
        for (unsigned int jj = 0; jj < face.size(); ++jj) {
            QVector3D vertexIndexTriple = face.at(jj);
            int positionIndex = (int) vertexIndexTriple.x() - 1;
            int textureIndex = (int) vertexIndexTriple.y() - 1;
            int normalIndex = (int) vertexIndexTriple.z() - 1;

            if (positionIndex >= positions.size() ||
                textureIndex >= textureCoordinates.size() ||
                normalIndex >= normals.size()) {
                throw std::invalid_argument("Face data includes out-of-bounds"
                                            " index for number of vertices, "
                                            "vertex texture, or vertex normal "
                                            "coordinates in memory.");
            }

            // Check if vertex has already been added to the list
            IndexedVertex* nextVertex = ((quickLookup[positionIndex])[textureIndex])[normalIndex];

            // If not, then construct the specified vertex and add it to both newOrdering and quickLookup
            if (!nextVertex) {
                QVector3D position = positions.at(positionIndex);
                QVector2D textureCoordinatePair = textureCoordinates.at(textureIndex);
                QVector3D normal = normals.at(normalIndex);

                nextVertex = new IndexedVertex(position, textureCoordinatePair, normal, positionIndex, textureIndex, normalIndex, newOrdering.size());
                (quickLookup[positionIndex])[textureIndex].emplace(normalIndex, nextVertex);
                newOrdering.append(nextVertex);
            }

            // Add the new index of the vertex, which either was looked up or just constructed
            faceIndices.append(nextVertex->newIndex_);
        }
    }

    return QPair<QVector<IndexedVertex*>, QVector<unsigned int>>(newOrdering, faceIndices);
}
