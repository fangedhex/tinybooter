#pragma once

#include <string>
#include <vector>
#include <reproc++/reproc.hpp>

class AsyncProcess
{
public:
  AsyncProcess(std::string cmd, std::vector<std::string> args);
  ~AsyncProcess();

  bool run();
  int stop();

private:
  std::vector<std::string> argv;
  reproc::process process;
  bool isRunning;
};
