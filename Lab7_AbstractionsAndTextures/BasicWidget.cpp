#include "BasicWidget.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), logger_(this)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
    for (auto renderable : renderables_) {
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

  qDebug() << QDir::currentPath();
  QString texFile = "../cat3.ppm";
  QVector<QVector3D> pos;
  QVector<QVector3D> norm;
  QVector<QVector2D> texCoord;
  QVector<unsigned int> idx;

  pos << QVector3D(-0.8, -0.8, 0.0);
  pos << QVector3D(0.8, -0.8, 0.0);
  pos << QVector3D(-0.8, 0.8, 0.0);
  pos << QVector3D(0.8, 0.8, 0.0);

  norm << QVector3D(0.0, 0.0, 1.0);
  norm << QVector3D(0.0, 0.0, 1.0);
  norm << QVector3D(0.0, 0.0, 1.0);
  norm << QVector3D(0.0, 0.0, 1.0);

  texCoord << QVector2D(0.0, 0.0);
  texCoord << QVector2D(1.0, 0.0);
  texCoord << QVector2D(0.0, 1.0);
  texCoord << QVector2D(1.0, 1.0);

  idx << 0 << 1 << 2 << 2 << 1 << 3;

  QMatrix4x4 leftTranslate = QMatrix4x4();
  leftTranslate.translate(-1.5, 0.0);

  QMatrix4x4 rightTranslate = QMatrix4x4();
  rightTranslate.translate(1.5, 0.0);

  Renderable* ren1 = new Renderable();
  ren1->init(pos, norm, texCoord, idx, texFile);
  ren1->setModelMatrix(leftTranslate);
  renderables_.push_back(ren1);

  Renderable* ren2 = new Renderable();
  ren2->init(pos, norm, texCoord, idx, texFile);
  ren2->setModelMatrix(rightTranslate);
  renderables_.push_back(ren2);

  glViewport(0, 0, width(), height());
  frameTimer_.start();
}

void BasicWidget::resizeGL(int w, int h)
{
    if (!logger_.isLogging()) {
        logger_.initialize();
        // Setup the logger for real-time messaging
        connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=]() {
            const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
            for (auto msg : messages) {
                qDebug() << msg;
            }
            });
        logger_.startLogging();
    }
  glViewport(0, 0, w, h);
  view_.setToIdentity();
  view_.lookAt(QVector3D(0.0f, 0.0f, 4.0f),
      QVector3D(0.0f, 0.0f, 0.0f),
      QVector3D(0.0f, 1.0f, 0.0f));
  projection_.setToIdentity();
  projection_.perspective(70.f, (float)w/(float)h, 0.001, 1000.0);
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  qint64 msSinceRestart = frameTimer_.restart();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto renderable : renderables_) {
      renderable->update(msSinceRestart);
      renderable->draw(view_, projection_);
  }
  update();
}
