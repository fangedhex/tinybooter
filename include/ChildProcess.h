#pragma once

#include <string>
#include <vector>
#include <future>
#include <reproc++/reproc.hpp>
#include <util/line_sink.h>

class ChildProcess
{
public:
  ChildProcess(std::string cmd, std::vector<std::string> args);
  ~ChildProcess();

  void setSync(line_sink s);

  bool run();
  std::future<bool> runAsync();

  int stop();

private:
  std::vector<std::string> argv;
  reproc::process process;
  bool isRunning;
  line_sink sink;
};
