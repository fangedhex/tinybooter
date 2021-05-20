#pragma once

#include <string>
#include <reproc++/reproc.hpp>
#include <spdlog/spdlog.h>

class line_sink
{
  std::string module;
  std::string str;

public:
  explicit line_sink(std::string module) noexcept : module(module) {}

  std::error_code operator()(reproc::stream stream, const uint8_t *buffer, size_t size)
  {
    (void)stream;

    for (size_t i = 0; i < size; i++)
    {
      char c = buffer[i];

      if (c != '\n')
      {
        str += c;
      }
      else
      {
        spdlog::info("[{}] {}", module, str);
        str = "";
      }
    }

    return {};
  }
};
