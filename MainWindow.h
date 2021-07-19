#pragma once

#include "BreakpointModel.h"
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
  void onRun();
  void onNext();
  void onStepDown();
  void onStepUp();

private:
  void updateView();
  void updateCodeBrowser();
  void updateBreakpointModel();
  void logMsg(const std::string &msg);

  Ui::MainWindow *ui;
  LldbDebugger debugger;
  std::string msgLog;
  BreakpointModel bpModel;
};

} // namespace visual_lldb
