#pragma once

#include <QVector2D>
#include <QVector3D>

/**
 * Represents a pair of position and texture coordinates for a vertex, for
 * re-indexing purposes.
 */
struct IndexedVertex {
    QVector3D position_;
    QVector2D textureCoordinates_;
    unsigned int positionIndex_;
    unsigned int textureCoordinatesIndex_;
    unsigned int newIndex_;

    /**
     * Standard parametrized constructor.
     */
    IndexedVertex(const QVector3D& position,
                  const QVector2D& textureCoordinates,
                  unsigned int positionIndex,
                  unsigned int textureCoordinatesIndex,
                  unsigned int newIndex) : position_(position),
                                           textureCoordinates_(textureCoordinates),
                                           positionIndex_(positionIndex),
                                           textureCoordinatesIndex_(textureCoordinatesIndex),
                                           newIndex_(newIndex) { }
};
