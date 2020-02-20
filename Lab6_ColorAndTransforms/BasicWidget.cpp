#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BasicWidget.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), logger_(this)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
  vbo_.release();
  vbo_.destroy();
  ibo_.release();
  ibo_.destroy();
  vao_.release();
  vao_.destroy();
}

//////////////////////////////////////////////////////////////////////
// Privates
QString BasicWidget::vertexShaderString() const
{
  QString str =
    "#version 330\n"
    "layout(location = 0) in vec3 position;\n"
    "layout(location = 1) in vec4 color;\n"

    "uniform mat4 modelMatrix;\n"
//    "uniform mat4 viewMatrix;\n"
//    "uniform mat4 projectionMatrix;\n"
    
    "out vec4 vertColor;\n"

    "void main()\n"
    "{\n"
    "  vec4 trans = modelMatrix * vec4(position, 1.0);\n"
    "  gl_Position = trans;\n"
    "  vertColor = color;\n"
    "}\n";
  return str;
}

QString BasicWidget::fragmentShaderString() const
{
  QString str =
	"#version 330\n"
    "in vec4 vertColor;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"  color = vertColor;\n"
	"}\n";
  return str;
}

void BasicWidget::createShader()
{
  QOpenGLShader vert(QOpenGLShader::Vertex);
  vert.compileSourceCode(vertexShaderString());
  QOpenGLShader frag(QOpenGLShader::Fragment);
  frag.compileSourceCode(fragmentShaderString());
  bool ok = shaderProgram_.addShader(&vert);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.addShader(&frag);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.link();
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
}
///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // Handle key events here.
  if (keyEvent->key() == Qt::Key_Left) {
    qDebug() << "Left Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else if (keyEvent->key() == Qt::Key_Right) {
    qDebug() << "Right Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else {
    qDebug() << "You Pressed an unsupported Key!";
  }
}
void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();
  logger_.initialize();

  // All of our matrices should be set to identity for now.
  model_.setToIdentity();
  view_.setToIdentity();
  projection_.setToIdentity();
  
  // Setup the logger for real-time messaging
  connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=](){
    const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
    for(auto msg : messages) {
      qDebug() << msg;
    }
  });
  logger_.startLogging();
  
  QOpenGLContext* curContext = this->context();
  qDebug() << "[BasicWidget]::initializeGL() -- Context Information:";
  qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
  qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
  qDebug() << "  Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  qDebug() << "  Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  qDebug() << "  Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << "  GLSL Version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Set up our shaders.
  createShader();

  // Define our verts
  static const GLfloat vertices[21] =
  {
    // positions       // colors
    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
   -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
  };

  // Define our indices
  static const GLuint idx[3] =
  {
      0, 1, 2
  };

  // Set up our buffers and our vao
  // Temporary bind of our shader.
  shaderProgram_.bind();

  vbo_.create();
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.bind();
  vbo_.allocate(vertices, 21 * sizeof(GL_FLOAT));

  ibo_.create();
  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.bind();
  ibo_.allocate(idx, 3 * sizeof(GL_UNSIGNED_INT));

  // Create a VAO to keep track of things for us.
  vao_.create();
  vao_.bind();
  vbo_.bind();

  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3, 7 * sizeof(GL_FLOAT));
  shaderProgram_.enableAttributeArray(1);
  shaderProgram_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GL_FLOAT), 4, 7 * sizeof(GL_FLOAT));

  ibo_.bind();
  // Releae the vao THEN the vbo
  vao_.release();
  shaderProgram_.release();

  glViewport(0, 0, width(), height());

  glm::mat4 model = glm::mat4(1.0f);
  GLint programID = shaderProgram_.programId();
  GLint modelID = glGetUniformLocation(programID, "modelMatrix");
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);

  // Field of View
  // float FoV = 45.0f;

  // glm::mat4 projection = glm::perspective(glm::radians(FoV), (float) w / (float) h, 0.1f, 100.0f);

  // //glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  // glm::mat4 view = glm::mat4(1.0f);
  // // note that we're translating the scene in the reverse direction of where we want to move
  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

  glm::mat4 model = glm::mat4(1.0f);
  //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));

  // QMatrix4x4 projectionQt = QMatrix4x4(glm::value_ptr(view)).transposed();
  // QMatrix4x4 viewQt = QMatrix4x4(glm::value_ptr(view)).transposed();
  // QMatrix4x4 modelQt = QMatrix4x4(glm::value_ptr(model)).transposed();

  // if (!printed_) {
  //   QVector4D row0 = modelQt.row(0);
  //   QVector4D row1 = modelQt.row(1);
  //   QVector4D row2 = modelQt.row(2);
  //   QVector4D row3 = modelQt.row(3);

  //   std::cout << std::endl;
  //   std::cout << row0.x() << " " << row0.y() << " " << row0.z() << " " << row0.w() << std::endl;
  //   std::cout << row1.x() << " " << row1.y() << " " << row1.z() << " " << row1.w() << std::endl;
  //   std::cout << row2.x() << " " << row2.y() << " " << row2.z() << " " << row2.w() << std::endl;
  //   std::cout << row3.x() << " " << row3.y() << " " << row3.z() << " " << row3.w() << std::endl;

  //   printed_ = true;
  // }

  // shaderProgram_.setUniformValue("modelMatrix", modelQt);
  // shaderProgram_.setUniformValue("viewMatrix", viewQt);
  // shaderProgram_.setUniformValue("projectionMatrix", projectionQt);

  GLint programID = shaderProgram_.programId();
  GLint modelID = glGetUniformLocation(programID, "modelMatrix");
  // GLint viewID = glGetUniformLocation(programID, "viewMatrix");
  // GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");
  
  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
  // glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
  // glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram_.bind();
  vao_.bind();
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
  vao_.release();
  shaderProgram_.release();
}
