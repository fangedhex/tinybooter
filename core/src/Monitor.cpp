#include <Monitor.h>
#include <spdlog/spdlog.h>
#include <App.h>

Monitor::Monitor(u_short port, App *app) : app(app) {
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
  if (app->getState() > AppState::Init) {
    res.status = 200;
    return;
  }

  for (auto job : app->getInitJobs()) {
    auto config = job->getConfig();
    if (job->getState() != JobState::SUCCESS) {
      res.status = 500;
    }
  }

  res.set_content("startup", "text/plain");
}

void Monitor::readiness(const httplib::Request &req, httplib::Response &res) {
  /*
  TODO As we don't have a way to know readiness of our system yet, we just use
  startup function
  */
  startup(req, res);
}

void Monitor::liveness(const httplib::Request &req, httplib::Response &res) {
  if (app->getState() != AppState::Running) {
    res.status = 500;
    return;
  }

  for (auto job : app->getDaemonJobs()) {
    auto config = job->getConfig();
    if (job->getState() != JobState::RUNNING) {
      res.status = 500;
    }
  }

  res.set_content("liveness", "text/plain");
}
