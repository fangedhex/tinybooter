#pragma once

#include <config/JobConfig.h>
#include <util/yaml_merge.h>

struct RunOnceJobConfig : public JobConfig
{
  uint32_t timeout = -1;
};

namespace YAML
{
  template <>
  struct convert<RunOnceJobConfig*>
  {
    /*static Node encode(const RunOnceJobConfig &rhs)
    {
      Node node;

      auto sub = YAML::convert<JobConfig>::encode(rhs);
      node = merge_nodes(node, sub);

      if (rhs.timeout > 0)
        node["timeout"] = rhs.timeout;

      return node;
    }*/

    static bool decode(const Node &node, RunOnceJobConfig*& rhs)
    {
      rhs = new RunOnceJobConfig();

      if (!node.IsMap())
      {
        return false;
      }

      //YAML::convert<JobConfig*>::decode(node, static_cast<JobConfig*&>(rhs));

      if (node["timeout"].IsDefined())
        rhs->timeout = node["timeout"].as<uint32_t>();

      return true;
    }
  };
}
