#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

#include "Renderable.h"
#include "Camera.h"

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  QMatrix4x4 world_;
  Camera camera_;
  
  QElapsedTimer frameTimer_;

  QVector<Renderable*> renderables_;

  QOpenGLDebugLogger logger_;
  bool isFilled_;
  // Mouse controls.
  enum MouseControl {NoAction = 0, Rotate, Zoom};
  QPoint lastMouseLoc_;
  MouseControl mouseAction_;
  float movementSpeed_;
  bool firstMouse_ = true;

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;
  void mousePressEvent(QMouseEvent* mouseEvent) override;
  void mouseMoveEvent(QMouseEvent* mouseEvent) override;
  void mouseReleaseEvent(QMouseEvent* mouseEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  
public:
  BasicWidget(QWidget* parent=nullptr);
  virtual ~BasicWidget();
  
  // Make sure we have some size that makes sense.
  QSize sizeHint() const override {return QSize(800,600);}
};
