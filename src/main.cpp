#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <Config.h>

int main(int argc, char const *argv[])
{
  fmt::print("Hello world !\n");
  spdlog::info("Hello world !");

  YAML::Node node = YAML::Load("url: test.com");
  Config cfg = node.as<Config>();

  spdlog::info("Url = {}", cfg.url);

  return 0;
}
