#pragma once

#include "BreakpointModel.h"
#include "FrameVariableModel.h"
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
  void onOpenFile();

private:
  void updateView();
  void updateCodeBrowser();
  void updateFrameModel();
  void updateBreakpointModel();
  void populateCodeBrowser(const std::string &fileName);
  void logMsg(const std::string &msg);

  Ui::MainWindow *ui;
  LldbDebugger debugger;
  std::string msgLog;
  FrameVariableModel frameModel;
  BreakpointModel bpModel;
  std::string currentFile;
};

} // namespace visual_lldb
