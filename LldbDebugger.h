#pragma once

#include <lldb/API/LLDB.h>

#include <string>
#include <vector>

namespace visual_lldb {

class LldbDebugger {
public:
  LldbDebugger(const std::string &targetPath);
  virtual ~LldbDebugger();

  void setBreakpoint(const std::string &functionName);
  void run();
  std::string getStacktrace();

private:
  lldb::SBDebugger debugger;
  lldb::SBTarget target;
  lldb::SBProcess process;
  std::vector<lldb::SBBreakpoint> bps;
};

} // namespace visual_lldb
