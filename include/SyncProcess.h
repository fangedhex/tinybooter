#pragma once

#include <string>
#include <vector>
#include <reproc++/run.hpp>

class SyncProcess
{
public:
  SyncProcess(std::initializer_list<const char *> args);
  SyncProcess(std::string cmd, std::vector<std::string> args);
  bool run();

private:
  std::vector<const char*> args;
};
