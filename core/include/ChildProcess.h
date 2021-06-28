#pragma once

#include <future>
#include <reproc++/reproc.hpp>
#include <string>
#include <util/line_sink.h>
#include <vector>

class ChildProcess {
public:
  ChildProcess(std::string cmd, std::vector<std::string> args);
  virtual ~ChildProcess();

  void setSync(line_sink s);

  virtual bool run();
  virtual int stop();

protected:
  ChildProcess() : sink("no-name"){};

private:
  std::vector<std::string> argv;
  reproc::process process;
  bool isRunning;
  line_sink sink;
};
