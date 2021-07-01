#pragma once

#include <App.h>
#include <Job.h>
#include <httplib.h>
#include <map>
#include <sys/types.h>
#include <thread>
#include <vector>

class Monitor {
public:
  Monitor(u_short port, App *app);
  ~Monitor();

  void stop();

private:
  std::thread *thread;
  App *app;
  httplib::Server svr;

  void startup(const httplib::Request &req, httplib::Response &res);
  void readiness(const httplib::Request &req, httplib::Response &res);
  void liveness(const httplib::Request &req, httplib::Response &res);
};
