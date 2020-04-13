#include "Renderable.h"
#include "ObjLoader.h"

#include <iostream>

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), diffuseMap_(QOpenGLTexture::Target2D), normalMap_(QOpenGLTexture::Target2D), numTris_(0), rotationAxis_(0.0, 1.0, 0.0), rotationSpeed_(0.125)
{
    rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
    if (diffuseMap_.isCreated()) {
        diffuseMap_.destroy();
    }
    if (normalMap_.isCreated()) {
        normalMap_.destroy();
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
    float* data = object->getData();
    unsigned int* indices = object->getIndices();

    unsigned int numData = object->getNumData();
    unsigned int numIndices = object->getNumIndices();
    
    unsigned int vertexSize = object->getVertexSize();
    QString diffuseMapPath = QString::fromStdString(object->getDiffuseMapPath());
    QString normalMapPath = QString::fromStdString(object->getNormalMapPath());

    diffuseMap_.setData(QImage(diffuseMapPath).mirrored(true, true));
    normalMap_.setData(QImage(normalMapPath).mirrored(true, true));

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

    // Position
    shader_.enableAttributeArray(0);
    shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize * sizeof(float));
    // UV
    shader_.enableAttributeArray(1);
    shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, vertexSize * sizeof(float));
    // Normal
    shader_.enableAttributeArray(2);
    shader_.setAttributeBuffer(2, GL_FLOAT, 5 * sizeof(float), 3, vertexSize * sizeof(float));
    // Tangent
    shader_.enableAttributeArray(3);
    shader_.setAttributeBuffer(3, GL_FLOAT, 8 * sizeof(float), 3, vertexSize * sizeof(float));

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

    shader_.setUniformValue("diffuseMap", 0);
    shader_.setUniformValue("normalMap", 1);

    shader_.setUniformValue("lightPos", QVector3D(0.0f, 2.0f, 3.0f));
    shader_.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 4.0f));

    vao_.bind();
    
    diffuseMap_.bind(0);
    normalMap_.bind(1);

    glDrawElements(GL_TRIANGLES, 3 * numTris_, GL_UNSIGNED_INT, 0);
    
    normalMap_.release(1);
    diffuseMap_.release(0);

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
