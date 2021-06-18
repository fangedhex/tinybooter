#pragma once

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

enum JobKind
{
  INIT,
  SERVICE,
  CLEANUP
};

inline const char *ToString(JobKind v)
{
  switch (v)
  {
  case INIT:
    return "Initialization";
  case SERVICE:
    return "Service";
  case CLEANUP:
    return "Clean up";
  default:
    return "[Unknown JobKind]";
  }
}

struct JobConfig
{
  JobKind kind;
  std::string name;
  std::string cmd;
  std::vector<std::string> args;
};

namespace YAML
{
  template <>
  struct convert<JobKind>
  {
    static Node encode(const JobKind &rhs)
    {
      Node node;

      switch (rhs)
      {
      case JobKind::INIT:
        node = "init";
        break;
      case JobKind::SERVICE:
        node = "service";
        break;
      case JobKind::CLEANUP:
        node = "cleanup";
        break;
      }

      return node;
    }

    static bool decode(const Node &node, JobKind &rhs)
    {
      std::string tmp = node.as<std::string>();

      if (tmp == "init")
      {
        rhs = JobKind::INIT;
      }
      else if (tmp == "service")
      {
        rhs = JobKind::SERVICE;
      }
      else if (tmp == "cleanup")
      {
        rhs = JobKind::CLEANUP;
      }

      return true;
    }
  };

  template <>
  struct convert<JobConfig>
  {
    static Node encode(const JobConfig &rhs)
    {
      Node node;

      node["kind"] = rhs.kind;
      node["name"] = rhs.name;
      node["cmd"] = rhs.cmd;
      node["args"] = rhs.args;

      return node;
    }

    static bool decode(const Node &node, JobConfig &rhs)
    {
      if (!node.IsMap())
      {
        return false;
      }

      rhs.kind = node["kind"].as<JobKind>();
      rhs.name = node["name"].as<std::string>();
      rhs.cmd = node["cmd"].as<std::string>();

      if (!node["args"].IsSequence())
      {
        return false;
      }
      rhs.args = node["args"].as<std::vector<std::string>>();

      return true;
    }
  };
}
