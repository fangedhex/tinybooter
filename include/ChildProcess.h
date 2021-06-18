#pragma once

#include <string>
#include <vector>
#include <future>
#include <reproc++/reproc.hpp>

class ChildProcess
{
public:
  ChildProcess(std::string cmd, std::vector<std::string> args);
  ~ChildProcess();

  bool run();
  std::future<bool> runAsync();

  int stop();

private:
  std::vector<std::string> argv;
  reproc::process process;
  bool isRunning;
};
