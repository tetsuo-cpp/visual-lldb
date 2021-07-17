#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include <lldb/API/LLDB.h>

#include <iostream>

namespace visual_lldb {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  // Call this before manipulating the window.
  ui->setupUi(this);

  // Create the debugger interface.
  QTextEdit *textEdit = new QTextEdit;
  textEdit->setReadOnly(true);
  QPushButton *bpButton = new QPushButton("Set breakpoint");
  QPushButton *runButton = new QPushButton("Run!");
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(textEdit);
  layout->addWidget(bpButton);
  layout->addWidget(runButton);
  QWidget *centralWidget = new QWidget;
  centralWidget->setLayout(layout);
  this->setCentralWidget(centralWidget);

  lldb::SBError error = lldb::SBDebugger::InitializeWithErrorHandling();
  if (error.Fail()) {
    std::cerr << "initialization failed: " << error.GetCString() << '\n';
    // return 1;
  }
  lldb::SBHostOS::ThreadCreated("<lldb.driver.main-thread>");

  lldb::SBDebugger dbg = lldb::SBDebugger::Create(false);
  auto target = dbg.CreateTarget("/home/tetsuo/dev/visual-lldb/foo");
  assert(target);
  auto bp = target.BreakpointCreateByName("printMsg");
  assert(bp);
  auto process = target.LaunchSimple(nullptr, nullptr, ".");
  assert(process);

  lldb::SBDebugger::Destroy(dbg);
  lldb::SBDebugger::Terminate();
}

MainWindow::~MainWindow() { delete ui; }

} // namespace visual_lldb
