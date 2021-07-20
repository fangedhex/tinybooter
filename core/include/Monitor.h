#pragma once

#include <Job.h>
#include <httplib.h>
#include <map>
#include <sys/types.h>
#include <thread>
#include <vector>
#include <JobsManager.h>

class Application;
class Monitor {
public:
  Monitor(u_short port, Application *app, JobsManager *jobsManager);
  ~Monitor();

  void stop();

private:
  std::thread *thread;
  Application *app;
  JobsManager *jobsManager;
  httplib::Server svr;

  void startup(const httplib::Request &req, httplib::Response &res);
  void readiness(const httplib::Request &req, httplib::Response &res);
  void liveness(const httplib::Request &req, httplib::Response &res);
};
