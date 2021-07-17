#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace visual_lldb {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), debugger("foo"), ui(new Ui::MainWindow) {
  // Call this before manipulating the window.
  ui->setupUi(this);

  QObject::connect(ui->bpButton, &QAbstractButton::clicked, this,
                   &MainWindow::onBreakpoint);
  QObject::connect(ui->runButton, &QAbstractButton::clicked, this,
                   &MainWindow::onRun);
  QObject::connect(ui->stacktraceButton, &QAbstractButton::clicked, this,
                   &MainWindow::onStacktrace);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onBreakpoint() { debugger.setBreakpoint("printMsg"); }

void MainWindow::onRun() { debugger.run(); }

void MainWindow::onStacktrace() {
  auto stacktrace = debugger.getStacktrace();
  auto *codeDoc = new QTextDocument(stacktrace.c_str());
  ui->codeBrowser->setDocument(codeDoc);
}

} // namespace visual_lldb
