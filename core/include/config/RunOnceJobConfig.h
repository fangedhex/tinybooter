#pragma once

#include <config/JobConfig.h>
#include <util/yaml_merge.h>

struct RunOnceJobConfig : public JobConfig
{
  u_int32_t timeout = -1;
};

namespace YAML
{
  template <>
  struct convert<RunOnceJobConfig>
  {
    static Node encode(const RunOnceJobConfig &rhs)
    {
      Node node;

      auto sub = YAML::convert<JobConfig>::encode(rhs);
      node = merge_nodes(node, sub);

      if (rhs.timeout > 0)
        node["timeout"] = rhs.timeout;

      return node;
    }

    static bool decode(const Node &node, RunOnceJobConfig &rhs)
    {
      if (!node.IsMap())
      {
        return false;
      }

      YAML::convert<JobConfig>::decode(node, rhs);

      if (node["timeout"].IsDefined())
        rhs.timeout = node["timeout"].as<u_int32_t>();

      return true;
    }
  };
}
