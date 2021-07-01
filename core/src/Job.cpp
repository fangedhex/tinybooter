#include <Job.h>

Job::Job(JobConfig *config, ChildProcessFactory childProcessFactory)
    : state(IDLE), config(config), thread(nullptr) {
  this->childProcess = childProcessFactory(config->cmd, config->args);
}

Job::Job()
    : state(IDLE), config(nullptr), thread(nullptr), childProcess(nullptr) {}

Job::~Job() {
  keepRunning = false;

  wait();

  delete thread;
  delete config;
  delete childProcess;
}

void Job::internalRun() {
  spdlog::info("Starting up job {}", config->name);
  do {
    state = RUNNING;
    if (childProcess->run()) {
      state = SUCCESS;
    } else {
      state = FAILURE;
    }

    if (keepRunning && config->kind == JobKind::SERVICE) {
      using namespace std::chrono_literals;
      spdlog::info("Job {} exited ! Restarting it in 3 seconds", config->name);
      std::this_thread::sleep_for(3000ms);
    }
  } while (keepRunning && config->kind == JobKind::SERVICE);
}

void Job::launch() {
  if (thread != nullptr)
    return;

  keepRunning = true;
  auto fn = std::bind(&Job::internalRun, this);
  thread = new std::thread(fn);
}

JobState Job::getState() const { return state; }

JobConfig Job::getConfig() const { return *config; }

void Job::stop() { keepRunning = false; }

void Job::wait() {
  if (thread && thread->joinable()) {
    thread->join();
  }
}
