#pragma once

#include <lldb/API/LLDB.h>

#include <string>
#include <vector>

namespace visual_lldb {

class CodeLocation {
public:
  explicit CodeLocation(std::string &&directory, std::string &&fileName,
                        size_t lineNumber)
      : directory(std::move(directory)), fileName(std::move(fileName)),
        lineNumber(lineNumber) {}
  const std::string &getDirectory() const;
  const std::string &getFileName() const;
  size_t getLine() const;

private:
  const std::string directory, fileName;
  size_t lineNumber;
};

class LldbDebugger {
public:
  explicit LldbDebugger(const std::string &targetPath);
  LldbDebugger(const LldbDebugger &) = delete;
  LldbDebugger &operator=(const LldbDebugger &) = delete;
  virtual ~LldbDebugger();
  void run();
  void resume();
  void next();
  void stepUp();
  void stepDown();
  void toggleBreakpoint(const std::string &fileName, size_t lineNumber);
  CodeLocation getLocation() const;
  std::vector<lldb::SBBreakpoint> &getBreakpoints();
  lldb::SBValueList getFrameVariables();
  bool isActive();

private:
  void waitForStop();
  bool isStopped();

  lldb::SBDebugger debugger;
  lldb::SBTarget target;
  lldb::SBProcess process;
  std::vector<lldb::SBBreakpoint> bps;
};

} // namespace visual_lldb
