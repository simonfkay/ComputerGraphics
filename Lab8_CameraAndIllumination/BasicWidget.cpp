#include "BasicWidget.h"

#include "UnitQuad.h"

#include <iostream>

#include <QtMath>

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent)
{
  logger_ = new QOpenGLDebugLogger(this);
  setFocusPolicy(Qt::StrongFocus);
  camera_.setPosition(QVector3D(0.5, 0.5, -2.0));
  camera_.setLookAt(QVector3D(0.5, 0.5, 0.0));
  world_.setToIdentity();
  movementSpeed_ = 0.05;
}

BasicWidget::~BasicWidget()
{
    makeCurrent();
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
  if (keyEvent->key() == Qt::Key_R) {
    camera_.setPosition(QVector3D(0.5, 0.5, -2.0));
    camera_.setLookAt(QVector3D(0.5, 0.5, 0.0));
    camera_.resetFov();
    camera_.calculatePerspective();
    update();
  } else if (keyEvent->key() == Qt::Key_W) {
    QVector3D gazeDirection = camera_.gazeVector();
    camera_.translateCamera(movementSpeed_ * gazeDirection);
    // Move forward
    update();
  } else if (keyEvent->key() == Qt::Key_A) {
    QVector3D gazeDirection = camera_.gazeVector();
    QVector3D upDirection = camera_.upVector();
    QVector3D rightDirection = QVector3D::crossProduct(gazeDirection, upDirection);
    camera_.translateCamera(-1 * movementSpeed_ * rightDirection);
    // Move left
    update();
  } else if (keyEvent->key() == Qt::Key_D) {
    QVector3D gazeDirection = camera_.gazeVector();
    QVector3D upDirection = camera_.upVector();
    QVector3D rightDirection = QVector3D::crossProduct(gazeDirection, upDirection);
    camera_.translateCamera(movementSpeed_ * rightDirection);
    // Move right
    update();
  } else if (keyEvent->key() == Qt::Key_S) {
    QVector3D gazeDirection = camera_.gazeVector();
    camera_.translateCamera(-1 * movementSpeed_ * gazeDirection);
    // Move back
    update();
  } else {
    qDebug() << "You Pressed an unsupported Key!";
  }
}

void BasicWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
  if (mouseEvent->button() == Qt::LeftButton) {
    mouseAction_ = Rotate;
  } else if (mouseEvent->button() == Qt::RightButton) {
    mouseAction_ = Zoom;
  }
  lastMouseLoc_ = mouseEvent->pos();
}

void BasicWidget::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  if (mouseAction_ == NoAction) {
    return;
  }
  QPoint delta = mouseEvent->pos() - lastMouseLoc_;
  lastMouseLoc_ = mouseEvent->pos();
  if (mouseAction_ == Rotate) {

    float sensitivity = 0.5f;
    float yaw = camera_.modifyYaw(sensitivity * delta.x());
    float pitch = -1 * camera_.modifyPitch(sensitivity * delta.y());

    yaw = qDegreesToRadians(yaw);
    pitch = qDegreesToRadians(pitch);


    QVector3D direction;
    direction.setX(qCos(yaw) * qCos(pitch));
    direction.setY(qSin(pitch));
    direction.setZ(qSin(yaw) * qCos(pitch));
    camera_.setGazeVector(direction);
  } else if (mouseAction_ == Zoom) {
    camera_.modifyFov(-1 * delta.y());
    camera_.calculatePerspective();
  } 
  update();
}

void BasicWidget::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    mouseAction_ = NoAction;
}

void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  qDebug() << QDir::currentPath();
  QString brickTex = "../brick.ppm";
  QString grassTex = "../grass.ppm";

  UnitQuad* backWall = new UnitQuad();
  backWall->init(brickTex);
  QMatrix4x4 backXform;
  backXform.setToIdentity();
  backXform.scale(1.0, 1.0, -1.0);
  backWall->setModelMatrix(backXform);
  renderables_.push_back(backWall);

  UnitQuad* rightWall = new UnitQuad();
  rightWall->init(brickTex);
  QMatrix4x4 rightXform;
  rightXform.setToIdentity();
  rightXform.rotate(90.0, 0.0, 1.0, 0.0);
  rightWall->setModelMatrix(rightXform);
  renderables_.push_back(rightWall);

  UnitQuad* leftWall = new UnitQuad();
  leftWall->init(brickTex);
  QMatrix4x4 leftXform;
  leftXform.setToIdentity();
  leftXform.translate(1.0, 0.0, -1.0);
  leftXform.rotate(-90.0, 0.0, 1.0, 0.0);
  leftWall->setModelMatrix(leftXform);
  renderables_.push_back(leftWall);

  UnitQuad* floor = new UnitQuad();
  floor->init(grassTex);
  QMatrix4x4 floorXform;
  floorXform.setToIdentity();
  floorXform.translate(-0.5, 0.0, 0.5);
  floorXform.scale(2.0, 2.0, 2.0);
  floorXform.rotate(-90.0, 1.0, 0.0, 0.0);
  floor->setModelMatrix(floorXform);
  renderables_.push_back(floor);

  glViewport(0, 0, width(), height());
  frameTimer_.start();
}

void BasicWidget::resizeGL(int w, int h)
{
    //bool madeCurrent = makeCurrent();
    //std::cout << "Made current context: " << madeCurrent << std::endl;
    QOpenGLContext* ctx = QOpenGLContext::currentContext();
    std::cout << "Context Extension: " << ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug")) << std::endl;
    if (!logger_->isLogging()) {
        bool isInitialized = logger_->initialize();
        std::cout << "Logger is initialized: " << isInitialized << std::endl;
        // Setup the logger for real-time messaging
        connect(logger_, &QOpenGLDebugLogger::messageLogged, [=]() {
            const QList<QOpenGLDebugMessage> messages = logger_->loggedMessages();
            for (auto msg : messages) {
                qDebug() << msg;
            }
            });
        logger_->startLogging();
    }
    glViewport(0, 0, w, h);

    camera_.setAspect((float)w / (float)h);
    camera_.calculatePerspective();
    glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  qint64 msSinceRestart = frameTimer_.restart();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  for (auto renderable : renderables_) {
      renderable->update(msSinceRestart);
      renderable->draw(world_, camera_.getViewMatrix(), camera_.getProjectionMatrix());
  }
  update();
}
