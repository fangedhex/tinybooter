#pragma once

#include <string>
#include <vector>
#include <reproc++/run.hpp>

class SyncProcess
{
public:
  SyncProcess(std::string cmd, std::vector<std::string> args);

  bool run();

private:
  std::vector<std::string> argv;
};
