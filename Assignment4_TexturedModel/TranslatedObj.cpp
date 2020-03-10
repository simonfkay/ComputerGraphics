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
 * Translates the data of an already loaded .obj file to a format that can be
 * easily used with OpenGL.
 * 
 * @param positions The list of distinct vertex positions for the object model.
 * @param textureCoordinates The list of distinct texture coordinates for the
 *                           object model.
 * @param faces The list of face data, which comprises of ordered triplets of
 *              index pairs, the first corresponding to the positions list, and
 *              the second corresponding to to the textureCoordinates list.
 * @param diffuseMapPath The file path to the corresponding diffuse map for
 *                       object texturing.
 * @return A pointer to an object containing the translated .obj data for use
 *         with OpenGL.
 * @throws invalid_argument if an index pair in one of the given faces lies
 *                          outside the bounds of either positions or
 *                          textureCoordinates.
 */
TranslatedObj* TranslatedObj::translate(const QVector<QVector3D>& positions, const QVector<QVector2D>& textureCoordinates, const QVector<QVector<QPair<int, int>>>& faces, const std::string& diffuseMapPath) {
    // Reorder vertex data to make sense for OpenGL
    QPair<QVector<IndexedVertex*>, QVector<unsigned int>> reorderedVertexData = TranslatedObj::reorderVertexData(positions, textureCoordinates, faces);
    QVector<IndexedVertex*> indexedVertices = reorderedVertexData.first;
    QVector<unsigned int> faceIndices = reorderedVertexData.second;

    // Initialize constants
    unsigned int numIndexedVertices = indexedVertices.size();
    unsigned int vertexSize = 3 + 2; // x, y, z, s, t
    unsigned int numData = numIndexedVertices * vertexSize;
    unsigned int numIndices = faceIndices.size();

    // Convert reordered vertex data to array
    float* data = new float[numData];
    for (unsigned int ii = 0; ii < numIndexedVertices; ++ii) {
        IndexedVertex* nextVertex = indexedVertices.at(ii);
        data[ii * vertexSize + 0] = nextVertex->position_.x();
        data[ii * vertexSize + 1] = nextVertex->position_.y();
        data[ii * vertexSize + 2] = nextVertex->position_.z();
        data[ii * vertexSize + 3] = nextVertex->textureCoordinates_.x();
        data[ii * vertexSize + 4] = nextVertex->textureCoordinates_.y();
    }

    unsigned int* indices = new unsigned int[numIndices];
    for (unsigned int ii = 0; ii < numIndices; ++ii) {
        indices[ii] = faceIndices.at(ii);
    }

    TranslatedObj* translated = new TranslatedObj(data, indices, numData, numIndices, vertexSize, diffuseMapPath);

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
                             const std::string& diffuseMapPath) : data_(data),
                                                                  indices_(indices),
                                                                  numData_(numData),
                                                                  numIndices_(numIndices),
                                                                  vertexSize_(vertexSize),
                                                                  diffuseMapPath_(diffuseMapPath) { }

/**
 * Reorders vertex data for use in OpenGL.
 * 
 * @param positions The list of distinct vertex positions for the object model.
 * @param textureCoordinates The list of distinct texture coordinates for the
 *                           object model.
 * @param faces The list of face data, which comprises of ordered triplets of
 *              index pairs, the first corresponding to the positions list, and
 *              the second corresponding to to the textureCoordinates list.
 * @return A pair of lists where the first is a list of the reordered vertex
 *         data, grouping together associated position and texture coordinate
 *         values, and the second is a list of these newly indexed vertices in
 *         the same order as specified in faces.
 * @throws invalid_argument if an index pair in one of the given faces lies
 *                          outside the bounds of either positions or
 *                          textureCoordinates.
 */
QPair<QVector<IndexedVertex*>, QVector<unsigned int>> TranslatedObj::reorderVertexData(const QVector<QVector3D>& positions, const QVector<QVector2D>& textureCoordinates, const QVector<QVector<QPair<int, int>>>& faces) {
    std::map<unsigned int, std::map<unsigned int, IndexedVertex*>> quickLookup;
    QVector<IndexedVertex*> newOrdering;
    QVector<unsigned int> faceIndices;

    for (const QVector<QPair<int, int>>& face : faces) {
        for (unsigned int ii = 0; ii < face.size(); ++ii) {
            QPair<int, int> vertexIndexPair = face.at(ii);
            int positionIndex = vertexIndexPair.first - 1;
            int textureIndex = vertexIndexPair.second - 1;

            if (positionIndex >= positions.size() ||
                textureIndex >= textureCoordinates.size()) {
                throw std::invalid_argument("Face data includes out-of-bounds"
                                            " index for number of vertices"
                                            " or vertex texure coordinates"
                                            " in memory.");
            }




            // Check if vertex has already been added to the list
            IndexedVertex* nextVertex = (quickLookup[positionIndex])[textureIndex];

            // If not, then construct the specified vertex and add it to voth newOrdering and quickLookup
            if (!nextVertex) {
                QVector3D position = positions.at(positionIndex);
                QVector2D textureCoordinatePair = textureCoordinates.at(textureIndex);
                nextVertex = new IndexedVertex(position, textureCoordinatePair, positionIndex, textureIndex, newOrdering.size());
                quickLookup[positionIndex].emplace(textureIndex, nextVertex);
                newOrdering.append(nextVertex);
            }

            // Add the new index of the vertex, which either was looked up or just constructed
            faceIndices.append(nextVertex->newIndex_);
        }
    }

    return QPair<QVector<IndexedVertex*>, QVector<unsigned int>>(newOrdering, faceIndices);
}
