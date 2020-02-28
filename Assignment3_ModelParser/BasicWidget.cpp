#include <iostream>

#include "BasicWidget.h"
#include "ObjLoader.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
}

//////////////////////////////////////////////////////////////////////
// Privates

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // switch (event->key()) {
  // case Qt::Key_Q:
  //   QCoreApplication::quit();
  //   break;
  // case Qt::Key_W:
  //   // TODO: Draw in wireframe mode
  //   break;
  // case Qt::Key_1:
  //   // TODO: Draw bunny (default)
  //   break;
  // case Qt::Key_2:
  //   // TODO: Draw monkey
  //   break;
  // default:
  //   qDebug() << "You Pressed an unsupported Key!";
  // }
}

void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  QOpenGLContext* curContext = this->context();
  qDebug() << "[BasicWidget]::initializeGL() -- Context Information:";
  qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
  qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
  qDebug() << "  Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  qDebug() << "  Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  qDebug() << "  Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << "  GLSL Version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  BasicWidget::createShaderProgram();
  BasicWidget::addArrayObject("../objects/octahedron.obj");

  glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
  projectionMatrix_.setToIdentity();
  projectionMatrix_.perspective(70.f, (float)w/(float)h, 0.001, 1000.0);
  shader_.setUniformValue("projectionMatrix", projectionMatrix_);

  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  std::cout << "Painting GL." << std::endl;
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  std::cout << "Binding shader." << std::endl;
  shader_.bind();
  std::cout << "Shader bound." << std::endl;
  vao_.bind();
  std::cout << "VAO bound." << std::endl;
  glDrawElements(GL_TRIANGLES, modelPolyCount_, GL_UNSIGNED_INT, 0);
  std::cout << "VAO drawn." << std::endl;

  vao_.release();
  std::cout << "VAO released." << std::endl;
  shader_.release();
  std::cout << "Shader released, GL painted." << std::endl;
}

// NOTE: Adapted from Lab 7
void BasicWidget::createShaderProgram() {
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

  shader_.enableAttributeArray(0);
  shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));

  QMatrix4x4 modelMatrix = QMatrix4x4();
  QMatrix4x4 viewMatrix = QMatrix4x4();
  viewMatrix.lookAt(QVector3D(0.0f, 0.0f, 5.0f),
                    QVector3D(0.0f, 0.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f));

  shader_.setUniformValue("modelMatrix", modelMatrix);
  shader_.setUniformValue("viewMatrix", viewMatrix);
}

// NOTE: Adapted from Lab 7
void BasicWidget::addArrayObject(std::string filePath) {
  ObjLoader fileLoader;
  fileLoader.loadFile(filePath);
  int numVertices = 0;
  int numIndices = 0;
  float* vertices = fileLoader.getVertices(numVertices);
  int* indices = fileLoader.getIndices(numIndices);

  vao_.create();
  vao_.bind();

  vbo_.create();
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.bind();
  vbo_.allocate(vertices, numVertices * sizeof(float));
  delete[] vertices;

  ibo_.create();
  ibo_.bind();
  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.allocate(indices, numIndices * sizeof(int));
  delete[] indices;

  vao_.release();
  vbo_.release();
  ibo_.release();

  modelPolyCount_ = numIndices / 3;
}
