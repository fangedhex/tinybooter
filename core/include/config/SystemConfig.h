#include <string>
#include <yaml-cpp/yaml.h>

#pragma once

struct SystemConfig
{
  unsigned short healthcheck_port = 8123;
  std::string jobs;
};

namespace YAML
{
  template <>
  struct convert<SystemConfig>
  {
    static Node encode(const SystemConfig &rhs)
    {
      Node node;
      node["healthcheck_port"] = rhs.healthcheck_port;
      node["jobs"] = rhs.jobs;
      return node;
    }

    static bool decode(const Node &node, SystemConfig &rhs)
    {
      if (!node.IsMap())
      {
        return false;
      }

      rhs.healthcheck_port = node["healthcheck_port"].as<unsigned short>();
      rhs.jobs = node["jobs"].as<std::string>();
      return true;
    }
  };
}
