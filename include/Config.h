#include <string>
#include <yaml-cpp/yaml.h>

#pragma once

struct Config
{
  std::string url;
};

namespace YAML
{
  template <>
  struct convert<Config>
  {
    static Node encode(const Config &rhs)
    {
      Node node;
      node["url"] = rhs.url;
      return node;
    }

    static bool decode(const Node &node, Config &rhs)
    {
      if (!node.IsMap())
      {
        return false;
      }

      rhs.url = node["url"].as<std::string>();
      return true;
    }
  };
}
