#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  QMatrix4x4 modelMatrix_;
  QMatrix4x4 viewMatrix_;
  QMatrix4x4 projectionMatrix_;
  QOpenGLShaderProgram shader_;
  QOpenGLVertexArrayObject vao_;
  QOpenGLBuffer vbo_;
  QOpenGLBuffer ibo_;

  
  unsigned int modelPolyCount_;

  void createShaderProgram();
  void addArrayObject(std::string filePath);

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  BasicWidget(QWidget* parent=nullptr);
  virtual ~BasicWidget();
  
  // Make sure we have some size that makes sense.
  QSize sizeHint() const {return QSize(800,600);}
};
