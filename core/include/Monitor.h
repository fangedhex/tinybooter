#pragma once

#include <Job.h>
#include <httplib.h>
#include <map>
#include <sys/types.h>
#include <thread>
#include <vector>

class Monitor {
public:
  Monitor(u_short port, std::vector<Job *> jobs);
  ~Monitor();

  void stop();

private:
  std::thread *thread;
  std::vector<Job *> jobs;
  httplib::Server svr;

  void startup(const httplib::Request &req, httplib::Response &res);
  void readiness(const httplib::Request &req, httplib::Response &res);
  void liveness(const httplib::Request &req, httplib::Response &res);
};
