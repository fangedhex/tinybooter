#include <JobsManager.h>

#include <config/DaemonConfig.h>
#include <config/RunOnceJobConfig.h>
#include <spdlog/spdlog.h>

JobsManager::JobsManager(BasicFactory<Job> *jobFactory)
: jobFactory(jobFactory)
{
}

JobsManager::~JobsManager()
{
  for (auto job : this->initJobs)
  {
    delete job;
  }
  for (auto job : this->daemonJobs)
  {
    delete job;
  }
  for (auto job : this->cleanJobs)
  {
    delete job;
  }
}

void JobsManager::createJobsFrom(std::vector<YAML::Node> jobsYaml)
{
  for(auto &jobYaml : jobsYaml)
  {
    // Create specialized job configuration depending on the job's kind
    auto kind = jobYaml["kind"].as<JobKind>();
    JobConfig* config;

    if (kind == JobKind::SERVICE)
    {
      config = jobYaml.as<DaemonConfig*>();
    }
    else
    {
      config = jobYaml.as<RunOnceJobConfig*>();
    }

    // TODO Maybe remove the needs of a factory function and just build and pass the childprocess directly ?
    auto childProcessFactory = [config](std::string cmd, Args args) -> ChildProcess* {
      auto childProcess = new ChildProcess(cmd, args);
      childProcess->setSync(line_sink(config->name));
      return childProcess;
    };

    auto job = jobFactory->create(config, childProcessFactory);

    switch(kind)
    {
      case JobKind::INIT:
        this->initJobs.push_back(job);
        break;
      case JobKind::SERVICE:
        this->daemonJobs.push_back(job);
        break;
      case JobKind::CLEANUP:
        this->cleanJobs.push_back(job);
        break;
    }
  }
}

void JobsManager::runOnce(JobKind kind)
{
  spdlog::debug("Launching jobs with type {} ...", ToString(kind));

  std::vector<Job*> jobs;
  if (kind == JobKind::INIT) {
    jobs = this->initJobs;
  } else {
    jobs = this->cleanJobs;
  }

  for (auto job : jobs)
  {
    job->launch();
    job->wait();
  }
}

void JobsManager::daemon()
{
  spdlog::debug("Launching deamon jobs ...");
  for (auto job : this->daemonJobs)
  {
    job->launch();
  }

  // We will wait until all daemon jobs exited
  for(auto job : this->daemonJobs) {
    job->wait();
  }
}

void JobsManager::stopAll()
{}

void JobsManager::waitAllDaemonJobsToFinish()
{
  for(auto job : this->daemonJobs)
  {
    job->wait();
  }
}

std::vector<Job *> JobsManager::getInitJobs() { return this->initJobs; }

std::vector<Job *> JobsManager::getDaemonJobs() { return this->daemonJobs; }

std::vector<Job *> JobsManager::getCleanJobs() { return this->cleanJobs; }
