#include "LldbDebugger.h"

#include <cassert>

namespace visual_lldb {

using namespace lldb;

const std::string &CodeLocation::getDirectory() const { return directory; }

const std::string &CodeLocation::getFileName() const { return fileName; }

size_t CodeLocation::getLine() const { return lineNumber; }

LldbDebugger::LldbDebugger(const std::string &targetPath) {
  SBError error = SBDebugger::InitializeWithErrorHandling();
  assert(!error.Fail());
  debugger = SBDebugger::Create();
  assert(debugger);
  target = debugger.CreateTarget(targetPath.c_str());
  assert(target);
}

LldbDebugger::~LldbDebugger() {
  SBDebugger::Destroy(debugger);
  SBDebugger::Terminate();
}

void LldbDebugger::run() {
  setBreakpoint("printMsg");
  process = target.LaunchSimple(nullptr, nullptr, ".");
  assert(process);
  waitForStop();
}

void LldbDebugger::next() {
  assert(process);
  auto thread = process.GetSelectedThread();
  thread.StepOver();
  waitForStop();
}

void LldbDebugger::stepDown() {
  assert(process);
  auto thread = process.GetSelectedThread();
  thread.StepInto();
  waitForStop();
}

void LldbDebugger::stepUp() {
  assert(process);
  auto thread = process.GetSelectedThread();
  thread.StepOut();
  waitForStop();
}

void LldbDebugger::setBreakpoint(const std::string &functionName) {
  auto bp = target.BreakpointCreateByName(functionName.c_str());
  assert(bp);
  bps.push_back(std::move(bp));
}

CodeLocation LldbDebugger::getLocation() const {
  assert(process);
  SBLineEntry lineEntry =
      process.GetSelectedThread().GetSelectedFrame().GetLineEntry();
  SBFileSpec fileSpec = lineEntry.GetFileSpec();
  return CodeLocation(fileSpec.GetDirectory(), fileSpec.GetFilename(),
                      lineEntry.GetLine());
}

std::vector<SBBreakpoint> &LldbDebugger::getBreakpoints() { return bps; }

bool LldbDebugger::isStopped() { return process.GetState() == eStateStopped; }

void LldbDebugger::waitForStop() {
  SBListener listener = debugger.GetListener();
  process.GetBroadcaster().AddListener(listener,
                                       SBProcess::eBroadcastBitStateChanged);
  while (1) {
    SBEvent event;
    if (listener.WaitForEvent(6, event)) {
      if (!event.IsValid())
        break;
      const uint32_t eventType = event.GetType();
      if (!SBProcess::EventIsProcessEvent(event))
        continue;
      if (eventType != SBProcess::eBroadcastBitStateChanged)
        continue;
      const StateType state = SBProcess::GetStateFromEvent(event);
      if (state == eStateStopped)
        break;
    }
  }
  assert(isStopped());
}

} // namespace visual_lldb