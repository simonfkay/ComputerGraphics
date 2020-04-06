#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

#include "Renderable.h"

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  QMatrix4x4 model_;
  QMatrix4x4 view_;
  QMatrix4x4 projection_;
  
  QElapsedTimer frameTimer_;

  QVector<Renderable*> renderables_;

  QOpenGLDebugLogger logger_;

  bool wireframeMode_;
  unsigned int modelSelectedIndex_;

  std::string input_;
  bool customInput_ = false;

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  BasicWidget(QWidget* parent=nullptr, std::string input="");
  virtual ~BasicWidget();
  
  // Make sure we have some size that makes sense.
  QSize sizeHint() const override {return QSize(800,600);}
};