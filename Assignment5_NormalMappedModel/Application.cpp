
#include "Application.h"

#include "BasicWidget.h"

Application::Application(QWidget* parent, std::string input) : QMainWindow(parent), input_(input)
{
  buildGui();
}

Application::~Application()
{}


void Application::buildGui()
{
  // A simple menubar.
  QMenuBar* menu = menuBar();
  QMenu* file = menu->addMenu("File");
  QAction* exit = file->addAction("Quit", [this]() {close();});

  // Our basic widget.
  BasicWidget* widget = new BasicWidget(this, input_);
  setCentralWidget(widget);
}
