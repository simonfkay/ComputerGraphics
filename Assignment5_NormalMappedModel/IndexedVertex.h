#pragma once

#include <QVector>
#include <QVector2D>
#include <QVector3D>

/**
 * Represents a tuple of position, normal, and texture coordinates for a
 * vertex, for re-indexing purposes.
 */
struct IndexedVertex {
    QVector3D position_;
    QVector2D textureCoordinates_;
    QVector3D normal_;
    unsigned int positionIndex_;
    unsigned int textureCoordinatesIndex_;
    unsigned int normalIndex_;
    unsigned int newIndex_;

    QVector<QVector3D> tangents_;

    /**
     * Standard parametrized constructor.
     */
    IndexedVertex(const QVector3D& position,
                  const QVector2D& textureCoordinates,
                  const QVector3D& normal,
                  unsigned int positionIndex,
                  unsigned int textureCoordinatesIndex,
                  unsigned int normalIndex,
                  unsigned int newIndex) : position_(position),
                                           textureCoordinates_(textureCoordinates),
                                           normal_(normal),
                                           positionIndex_(positionIndex),
                                           textureCoordinatesIndex_(textureCoordinatesIndex),
                                           normalIndex_(normalIndex),
                                           newIndex_(newIndex) { }

    /**
     * Adds a tangent vector to this vertex's list, to be averaged out later.
     *
     * @param tangent The tangent vector to be added.
     */
    void addTangent(const QVector3D& tangent) {
        tangents_.push_back(tangent);
    }

    /**
     * Average out all the added tangent vectors for this vertex.
     *
     * @returns The final averaged out tangent vector for this vertex.
     */
    QVector3D getFinalTangent() {
        QVector3D tangentSum;
        for (int ii = 0; ii < tangents_.size(); ++ii) {
            tangentSum += tangents_.at(ii);
        }
        return tangentSum / tangents_.size();
    }

};
