#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <fstream>

namespace visual_lldb {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), debugger("foo"), ui(new Ui::MainWindow) {
  // Call this before manipulating the window.
  ui->setupUi(this);

  ui->frameView->setModel(&frameModel);
  ui->bpView->setModel(&bpModel);

  QObject::connect(ui->runButton, &QAbstractButton::clicked, this,
                   &MainWindow::onRun);
  QObject::connect(ui->nextButton, &QAbstractButton::clicked, this,
                   &MainWindow::onNext);
  QObject::connect(ui->stepDownButton, &QAbstractButton::clicked, this,
                   &MainWindow::onStepDown);
  QObject::connect(ui->stepUpButton, &QAbstractButton::clicked, this,
                   &MainWindow::onStepUp);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onRun() {
  debugger.run();
  updateView();
}

void MainWindow::onNext() {
  debugger.next();
  updateView();
}

void MainWindow::onStepDown() {
  debugger.stepDown();
  updateView();
}

void MainWindow::onStepUp() {
  debugger.stepUp();
  updateView();
}

void MainWindow::updateView() {
  updateCodeBrowser();
  updateBreakpointModel();
  updateFrameModel();
}

void MainWindow::updateCodeBrowser() {
  const auto codeLoc = debugger.getLocation();
  // Do something smarter than this.
  const std::string filePath =
      codeLoc.getDirectory() + '/' + codeLoc.getFileName();
  // Read the contents off the disk and populate the code view.
  std::ifstream file(filePath);
  assert(file);
  const std::string contents((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
  auto *codeDoc = new QTextDocument(contents.c_str());
  ui->codeBrowser->setDocument(codeDoc);
  // Mark the breakpoints visually as well as the current position.
}

void MainWindow::updateFrameModel() {
  auto frameVariables = debugger.getFrameVariables();
  std::vector<FrameVariable> modelFrame;
  for (int i = 0; i < frameVariables.GetSize(); ++i) {
    auto val = frameVariables.GetValueAtIndex(i);
    FrameVariable newFrame;
    newFrame.name = val.GetName();
    newFrame.value = val.GetValue();
    modelFrame.push_back(std::move(newFrame));
  }
  logMsg("Displaying " + std::to_string(modelFrame.size()) +
         " frame variables");
  frameModel.setFrameVariables(std::move(modelFrame));
}

void MainWindow::updateBreakpointModel() {
  // Debugger should just return this instead of doing transformation here.
  auto &debuggerBps = debugger.getBreakpoints();
  std::vector<Breakpoint> modelBps;
  std::transform(debuggerBps.begin(), debuggerBps.end(),
                 std::back_inserter(modelBps), [](lldb::SBBreakpoint &bp) {
                   Breakpoint modelBp;
                   lldb::SBAddress addr = bp.GetLocationAtIndex(0).GetAddress();
                   lldb::SBLineEntry lineEntry = addr.GetLineEntry();
                   lldb::SBFileSpec fileSpec = lineEntry.GetFileSpec();
                   modelBp.filePath = std::string(fileSpec.GetDirectory()) +
                                      '/' + fileSpec.GetFilename();
                   modelBp.lineNumber = lineEntry.GetLine();
                   return modelBp;
                 });
  logMsg("Added " + std::to_string(modelBps.size()) + " breakpoints");
  bpModel.setBreakpoints(std::move(modelBps));
}

void MainWindow::logMsg(const std::string &msg) {
  // I'm a Qt noob so let's do this. It'll be helpful for debugging so I don't
  // need to keep printing stuff to stdout.
  msgLog.append(msg);
  msgLog.push_back('\n');
  ui->logView->setPlainText(msgLog.c_str());
}

} // namespace visual_lldb
