#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <util/glob.h>
#include <SystemConfig.h>
#include <JobConfig.h>

int main(int argc, char const *argv[])
{
  spdlog::set_level(spdlog::level::debug);

  YAML::Node node = YAML::LoadFile("example/config.yaml");
  SystemConfig cfg = node.as<SystemConfig>();

  spdlog::debug("Port = {}", cfg.healthcheck_port);
  spdlog::debug("Jobs glob = {}", cfg.jobs);

  for (auto &p : glob::rglob(cfg.jobs))
  {
    spdlog::debug("File = {}", p.string());

    YAML::Node job_node = YAML::LoadFile(p);
    JobConfig job_cfg = job_node.as<JobConfig>();

    spdlog::debug("kind = {}", ToString(job_cfg.kind));
  }

  return 0;
}
