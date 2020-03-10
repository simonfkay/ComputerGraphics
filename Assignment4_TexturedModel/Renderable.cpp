#include "Renderable.h"
#include "ObjLoader.h"

#include <iostream>

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), numTris_(0), rotationAxis_(0.0, 1.0, 0.0), rotationSpeed_(0.25)
{
    rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
    if (texture_.isCreated()) {
        texture_.destroy();
    }
    if (vbo_.isCreated()) {
        vbo_.destroy();
    }
    if (ibo_.isCreated()) {
        ibo_.destroy();
    }
    if (vao_.isCreated()) {
        vao_.destroy();
    }
}

void Renderable::createShaders()
{
    QString vertexFilename = "../vert.glsl";
    bool ok = shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFilename);
    if (!ok) {
        qDebug() << shader_.log();
    }
    QString fragmentFilename = "../frag.glsl";
    ok = shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFilename);
    if (!ok) {
        qDebug() << shader_.log();
    }
    ok = shader_.link();
    if (!ok) {
        qDebug() << shader_.log();
    }
}

void Renderable::init(TranslatedObj* object)
{
    unsigned int numIndices = object->getNumIndices();
    unsigned int numData = object->getNumData();
    float* data = object->getData();
    unsigned int* indices = object->getIndices();
    unsigned int vertexSize = object->getVertexSize();
    QString diffuseMapPath = QString::fromStdString(object->getDiffuseMapPath());

    texture_.setData(QImage(diffuseMapPath));

    // Set our model matrix to identity
    modelMatrix_.setToIdentity();

    // Set our number of trianges.
    numTris_ = numIndices / 3;

    // num verts (used to size our vbo)
    int numVerts = numData / vertexSize;

    // Setup our shader.
    createShaders();

    // Now we can set up our buffers.
    // The VBO is created -- now we must create our VAO
    vao_.create();
    vao_.bind();
    vbo_.create();
    vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo_.bind();
    vbo_.allocate(data, numData * sizeof(float));

    // Create our index buffer
    ibo_.create();
    ibo_.bind();
    ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // create a temporary array for our indexes
    ibo_.allocate(indices, numIndices * sizeof(unsigned int));

    // Make sure we setup our shader inputs properly
    shader_.enableAttributeArray(0);
    shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize * sizeof(float));
    shader_.enableAttributeArray(1);
    shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, vertexSize * sizeof(float));

    // Release our vao and THEN release our buffers.
    vao_.release();
    vbo_.release();
    ibo_.release();

    delete object;
}

void Renderable::update(const qint64 msSinceLastFrame)
{
    // For this lab, we want our polygon to rotate. 
    float sec = msSinceLastFrame / 1000.0f;
    float anglePart = sec * rotationSpeed_ * 360.f;
    rotationAngle_ += anglePart;
    while (rotationAngle_ >= 360.0) {
        rotationAngle_ -= 360.0;
    }
}

void Renderable::draw(const QMatrix4x4& view, const QMatrix4x4& projection)
{
    // Create our model matrix.
    QMatrix4x4 rotMatrix;
    rotMatrix.setToIdentity();
    rotMatrix.rotate(rotationAngle_, rotationAxis_);

    QMatrix4x4 modelMat = modelMatrix_ * rotMatrix;
    // Make sure our state is what we want
    shader_.bind();
    // Set our matrix uniforms!
    shader_.setUniformValue("modelMatrix", modelMat);
    shader_.setUniformValue("viewMatrix", view);
    shader_.setUniformValue("projectionMatrix", projection);

    vao_.bind();
    texture_.bind();
    glDrawElements(GL_TRIANGLES, 3 * numTris_, GL_UNSIGNED_INT, 0);
    texture_.release();
    vao_.release();
    shader_.release();
}

void Renderable::setModelMatrix(const QMatrix4x4& transform)
{
    modelMatrix_ = transform;
}

void Renderable::setRotationAxis(const QVector3D& axis)
{
    rotationAxis_ = axis;
}

void Renderable::setRotationSpeed(float speed)
{
    rotationSpeed_ = speed;
}

Renderable* Renderable::createFromFile(const std::string& filePath) {
    Renderable* renderable = new Renderable();
    ObjLoader* loader = ObjLoader::getInstance();
    loader->loadFile(filePath);
    TranslatedObj* object = loader->translate();
    loader->clear();
    renderable->init(object);
    return renderable;
}
