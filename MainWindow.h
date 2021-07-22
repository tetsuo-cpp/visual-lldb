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
  explicit MainWindow(const QString &executablePath, QWidget *parent = nullptr);
  virtual ~MainWindow();

public slots:
  void onRun();
  void onNext();
  void onStepDown();
  void onStepUp();
  void onOpenFile();

private:
  void updateView();
  void updateCodeBrowser(const std::vector<Breakpoint> &bps);
  void updateFrameModel();
  std::vector<Breakpoint> updateBreakpointModel();
  void populateCodeBrowser(const std::string &fileName, size_t lineNumber,
                           const std::vector<Breakpoint> &bps);
  void logMsg(const std::string &msg);

  Ui::MainWindow *ui;
  LldbDebugger debugger;
  std::string msgLog;
  FrameVariableModel frameModel;
  BreakpointModel bpModel;
  std::string currentFile;
};

} // namespace visual_lldb
