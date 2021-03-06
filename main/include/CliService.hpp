/**
 * Handle unix socket for cli like connections
 */

#pragma once

#include <cstring>
#include <vector>
#include <thread>

#include "Service.hpp"
//#include "shared.h"

typedef unsigned short u_short;
class CliService
{
  ILogger &logger;
  std::vector<Service> &services;
  std::thread *th;
  int sock;
  void thread_func();
  bool keep_running;
  CliService(std::vector<Service> &services, ILogger &logger);

public:
  CliService(std::string path, std::vector<Service> &services, ILogger &logger);
  CliService(u_short port, std::vector<Service> &services, ILogger &logger);
  ~CliService();
  void launch();
  void kill();
};
