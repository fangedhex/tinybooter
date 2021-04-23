#include <yaml-cpp/yaml.h>

inline const YAML::Node &cnode(const YAML::Node &n)
{
  return n;
}

inline YAML::Node merge_nodes(YAML::Node a, YAML::Node b)
{
  if (!b.IsMap())
  {
    // If b is not a map, merge result is b, unless b is null
    return b.IsNull() ? a : b;
  }
  if (!a.IsMap())
  {
    // If a is not a map, merge result is b
    return b;
  }
  if (!b.size())
  {
    // If a is a map, and b is an empty map, return a
    return a;
  }
  // Create a new map 'c' with the same mappings as a, merged with b
  auto c = YAML::Node(YAML::NodeType::Map);
  for (auto n : a)
  {
    if (n.first.IsScalar())
    {
      const std::string &key = n.first.Scalar();
      auto t = YAML::Node(cnode(b)[key]);
      if (t)
      {
        c[n.first] = merge_nodes(n.second, t);
        continue;
      }
    }
    c[n.first] = n.second;
  }
  // Add the mappings from 'b' not already in 'c'
  for (auto n : b)
  {
    if (!n.first.IsScalar() || !cnode(c)[n.first.Scalar()])
    {
      c[n.first] = n.second;
    }
  }
  return c;
}
