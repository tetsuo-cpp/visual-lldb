#include "LldbDebugger.h"

#include <cassert>

namespace visual_lldb {

using namespace lldb;

LldbDebugger::LldbDebugger(const std::string &targetPath) {
  lldb::SBError error = lldb::SBDebugger::InitializeWithErrorHandling();
  assert(!error.Fail());
  // lldb::SBHostOS::ThreadCreated("<lldb.driver.main-thread>");
  debugger = SBDebugger::Create();
  assert(debugger);
  target = debugger.CreateTarget(targetPath.c_str());
  assert(target);
}

LldbDebugger::~LldbDebugger() {
  SBDebugger::Destroy(debugger);
  SBDebugger::Terminate();
}

void LldbDebugger::setBreakpoint(const std::string &functionName) {
  auto bp = target.BreakpointCreateByName(functionName.c_str());
  assert(bp);
  bps.push_back(std::move(bp));
}

void LldbDebugger::run() {
  process = target.LaunchSimple(nullptr, nullptr, ".");
  assert(process);
}

// Can't really figure out how to get a full stacktrace like what you'd get with
// `bt`. Let's just get the function that it broke on.
std::string LldbDebugger::getStacktrace() {
  auto thread = process.GetSelectedThread();
  assert(thread);
  auto frame = thread.GetSelectedFrame();
  assert(frame);
  return frame.GetFunctionName();
}

} // namespace visual_lldb
