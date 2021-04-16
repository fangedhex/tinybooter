#include <fmt/core.h>
#include <spdlog/spdlog.h>

int main(int argc, char const *argv[])
{
  fmt::print("Hello world !\n");
  spdlog::info("Hello world !");
  return 0;
}
