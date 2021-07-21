#include "MainWindow.h"

#include <QApplication>

#include <iostream>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  // Get the program we're debugging.
  //
  // In the future, we should let users switch executables within the GUI as
  // well as use it with core dumps. For now, let's just pass in the executable
  // as the first arg.
  const auto &argList = QCoreApplication::arguments();
  if (argList.size() != 2) {
    std::cerr << "Usage: visual-lldb <program>\n";
    return EINVAL;
  }
  const QString executablePath = argList.at(1);
  visual_lldb::MainWindow w(executablePath);
  w.show();
  return a.exec();
}
