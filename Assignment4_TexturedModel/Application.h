/**
 * The primary application code.
 */

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

class Application : public QMainWindow
{
  Q_OBJECT

public:
  Application(QWidget* parent=0, std::string input="");
  virtual ~Application();
  
signals:

public slots:

private:
  void buildGui();

  std::string input_;
};
