#include <iostream>

#include "BasicWidget.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
  for (Renderable* renderable : renderables_) {
    delete renderable;
  }
  renderables_.clear();
}

//////////////////////////////////////////////////////////////////////
// Privates

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  switch (keyEvent->key()) {
  case Qt::Key_Q:
    qDebug() << "Q key pressed.";
    QCoreApplication::quit();
    break;
  case Qt::Key_W:
    qDebug() << "W key pressed.";
    wireframeMode_ = !wireframeMode_;
    break;
  case Qt::Key_1:
    qDebug() << "1 key pressed.";
    modelSelectedIndex_ = 0;
    update();
    break;
  case Qt::Key_2:
    qDebug() << "2 key pressed.";
    // modelSelectedIndex_ = 1; TODO: Add multiple objects to load
    update();
    break;
  default:
    qDebug() << "You pressed an unsupported key.";

  }
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

  wireframeMode_ = false;
  modelSelectedIndex_ = 0;
  renderables_.push_back(Renderable::createFromFile("../objects/house/house_obj.obj"));

  glViewport(0, 0, width(), height());
  frameTimer_.start();
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  view_.setToIdentity();
  view_.lookAt(QVector3D(0.0f, 0.0f, 2.0f),
      QVector3D(0.0f, 0.0f, 0.0f),
      QVector3D(0.0f, 1.0f, 0.0f));
  projection_.setToIdentity();
  projection_.perspective(70.f, (float)w/(float)h, 0.001, 1000.0);
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  qint64 msSinceRestart = frameTimer_.restart();
  // std::cout << "Painting GL." << std::endl;
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (wireframeMode_) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  } else {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }

  renderables_.at(modelSelectedIndex_)->update(msSinceRestart);
  renderables_.at(modelSelectedIndex_)->draw(view_, projection_);

  update();
}
