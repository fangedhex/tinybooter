#include <Monitor.h>

Monitor::Monitor(u_short port)
{
  using namespace std::placeholders;

  svr.Get("/startup", std::bind(&Monitor::startup, this, _1, _2));
  svr.Get("/readiness", std::bind(&Monitor::readiness, this, _1, _2));
  svr.Get("/liveness", std::bind(&Monitor::liveness, this, _1, _2));

  svr.listen("0.0.0.0", port);
}

void Monitor::updateJobStatus(u_short jobId, JobStatus newStatus)
{
  jobStatus[jobId] = newStatus;
}

void Monitor::startup(const httplib::Request &req, httplib::Response &res)
{
  res.set_content("startup", "text/plain");
}

void Monitor::readiness(const httplib::Request &req, httplib::Response &res)
{
  res.set_content("readiness", "text/plain");
}

void Monitor::liveness(const httplib::Request &req, httplib::Response &res)
{
  res.set_content("liveness", "text/plain");
}
