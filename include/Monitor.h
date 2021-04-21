#pragma once

#include <sys/types.h>
#include <map>
#include <lib/httplib.h>

enum JobStatus
{
  STARTED,
  RUNNING,
  EXITED
};

class Monitor
{
public:
  Monitor(u_short port);
  void updateJobStatus(u_short jobId, JobStatus newStatus);

private:
  std::map<u_short, JobStatus> jobStatus;
  httplib::Server svr;

  void startup(const httplib::Request &req, httplib::Response &res);
  void readiness(const httplib::Request &req, httplib::Response &res);
  void liveness(const httplib::Request &req, httplib::Response &res);
};
