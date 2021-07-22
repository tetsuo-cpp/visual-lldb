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
  // Restart the run.
  if (process)
    process.Destroy();
  process = target.LaunchSimple(nullptr, nullptr, ".");
  assert(process);
  waitForStop();
}

void LldbDebugger::resume() {
  assert(process);
  process.Continue();
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

void LldbDebugger::toggleBreakpoint(const std::string &fileName,
                                    size_t lineNumber) {
  for (auto it = bps.begin(); it != bps.end(); ++it) {
    auto &bp = *it;
    auto lineEntry = bp.GetLocationAtIndex(0).GetAddress().GetLineEntry();
    auto fileSpec = lineEntry.GetFileSpec();
    auto bpFileName =
        std::string(fileSpec.GetDirectory()) + '/' + fileSpec.GetFilename();
    // The breakpoint exists. Delete it.
    if (fileName == bpFileName && lineEntry.GetLine() == lineNumber) {
      const break_id_t bpId = bp.GetID();
      target.BreakpointDelete(bpId);
      bps.erase(it);
      return;
    }
  }
  // The breakpoint doesn't exist, add it.
  auto bp = target.BreakpointCreateByLocation(fileName.c_str(), lineNumber);
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

SBValueList LldbDebugger::getFrameVariables() {
  assert(process);
  return process.GetSelectedThread().GetSelectedFrame().GetVariables(
      true, true, true, true, DynamicValueType::eNoDynamicValues);
}

bool LldbDebugger::isActive() {
  return process && process.GetState() == eStateStopped;
}

bool LldbDebugger::isStopped() {
  return process.GetState() == eStateStopped ||
         process.GetState() == eStateExited;
}

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
      if (state == eStateStopped || state == eStateExited)
        break;
    }
  }
  assert(isStopped());
}

} // namespace visual_lldb
