/**
 * Support code written by Erik W. Anderson
 */

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "Application.h"

int main(int argc, char** argv) {
  QApplication a(argc, argv);
  QString appDir = a.applicationDirPath();
  QDir::setCurrent(appDir);

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setVersion(3,3);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(fmt);

  std::string input;
  if (argc == 2) {
    input = argv[1];
  } else {
    input = "";
  }

  Application app(0, input);
  app.show();
  return QApplication::exec();
}
