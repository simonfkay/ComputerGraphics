#include "Renderable.h"
#include "ObjLoader.h"

#include <iostream>

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 1.0, 0.0), rotationSpeed_(0.25)
{
    rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
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

void Renderable::init(const QVector<QVector3D>& positions, const QVector<QVector3D>& normals, const QVector<unsigned int>& indexes)
{
    // NOTE:  We do not currently do anything with normals -- we just
    // have it here for a later implementation!
    // We need to make sure our sizes all work out ok.
    // if (positions.size() != normals.size()) {
    //     qDebug() << "[Renderable]::init() -- positions size mismatch with normal coordinates";
    //     return;
    // }

    // Set our model matrix to identity
    modelMatrix_.setToIdentity();

    // Set our number of trianges.
    numTris_ = indexes.size() / 3;

    // num verts (used to size our vbo)
    int numVerts = positions.size();
    vertexSize_ = 3;  // Position + texCoord
    int numVBOEntries = numVerts * vertexSize_;

    // Setup our shader.
    createShaders();

    // Now we can set up our buffers.
    // The VBO is created -- now we must create our VAO
    vao_.create();
    vao_.bind();
    vbo_.create();
    vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo_.bind();
    // Create a temporary data array
    float* data = new float[numVBOEntries];
    for (int ii = 0; ii < numVerts; ++ii) {
        data[ii * vertexSize_ + 0] = positions.at(ii).x();
        data[ii * vertexSize_ + 1] = positions.at(ii).y();
        data[ii * vertexSize_ + 2] = positions.at(ii).z();
    }
    vbo_.allocate(data, numVBOEntries * sizeof(float));
    delete[] data;

    // Create our index buffer
    ibo_.create();
    ibo_.bind();
    ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // create a temporary array for our indexes
    unsigned int* idxAr = new unsigned int[indexes.size()];
    for (int i = 0; i < indexes.size(); ++i) {
        idxAr[i] = indexes.at(i) - 1;
    }
    ibo_.allocate(idxAr, indexes.size() * sizeof(unsigned int));
    delete[] idxAr;

    // Make sure we setup our shader inputs properly
    shader_.enableAttributeArray(0);
    shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));

    // Release our vao and THEN release our buffers.
    vao_.release();
    vbo_.release();
    ibo_.release();
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
    glDrawElements(GL_TRIANGLES, 3 * numTris_, GL_UNSIGNED_INT, 0);
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

Renderable* Renderable::createFromFile(std::string filePath) {
    Renderable* renderable = new Renderable();
    ObjLoader loader;
    loader.loadFile(filePath);
    renderable->init(loader.getVertices(), loader.getVertexNormals(), loader.getIndices());
    return renderable;
}
