#pragma once

#include "LldbDebugger.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace visual_lldb {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

public slots:
  void onBreakpoint();
  void onRun();
  void onStacktrace();

private:
  Ui::MainWindow *ui;
  LldbDebugger debugger;
};

} // namespace visual_lldb
