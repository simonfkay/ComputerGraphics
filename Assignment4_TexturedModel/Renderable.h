#pragma once

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "TranslatedObj.h"

class Renderable {
protected:
    // Each renderable has its own model matrix
    QMatrix4x4 modelMatrix_;
    // For now, we have only one shader per object
    QOpenGLShaderProgram shader_;
    // For now, we have only one texture per object
    QOpenGLTexture texture_;
    // For now, we have a single unified buffer per object
    QOpenGLBuffer vbo_;
    // Make sure we have an index buffer.
    QOpenGLBuffer ibo_;
    // We have a single draw call, so a single vao
    QOpenGLVertexArrayObject vao_;
    // Keep track of how many triangles we actually have to draw in our ibo
    unsigned int numTris_;

    // Define our axis of rotation for animation
    QVector3D rotationAxis_;
    float rotationSpeed_;
    float rotationAngle_;

    // Create our shader and fix it up
    void createShaders();

public:
    Renderable();
    virtual ~Renderable();

    virtual void init(TranslatedObj* object);
    virtual void update(const qint64 msSinceLastFrame);
    virtual void draw(const QMatrix4x4& view, const QMatrix4x4& projection);

    void setModelMatrix(const QMatrix4x4& transform);
    void setRotationAxis(const QVector3D& axis);
    void setRotationSpeed(float speed);

    static Renderable* createFromFile(const std::string& filePath);

private:

};