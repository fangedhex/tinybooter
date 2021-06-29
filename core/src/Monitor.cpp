#include <Monitor.h>
#include <spdlog/spdlog.h>

Monitor::Monitor(u_short port, std::vector<Job *> jobs) : jobs(jobs) {
  using namespace std::placeholders;

  svr.Get("/startup", std::bind(&Monitor::startup, this, _1, _2));
  svr.Get("/readiness", std::bind(&Monitor::readiness, this, _1, _2));
  svr.Get("/liveness", std::bind(&Monitor::liveness, this, _1, _2));

  spdlog::info("Launching server on port {}", port);

  thread =
      new std::thread([this, port]() { this->svr.listen("localhost", port); });

  while (!svr.is_running()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

Monitor::~Monitor() {
  stop();
  delete thread;
}

void Monitor::stop() {
  svr.stop();
  thread->join();
}

void Monitor::startup(const httplib::Request &req, httplib::Response &res) {
  res.set_content("startup", "text/plain");
}

void Monitor::readiness(const httplib::Request &req, httplib::Response &res) {
  res.set_content("readiness", "text/plain");
}

void Monitor::liveness(const httplib::Request &req, httplib::Response &res) {
  res.set_content("liveness", "text/plain");
}
