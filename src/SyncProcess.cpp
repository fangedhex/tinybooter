#include <SyncProcess.h>

#include <spdlog/spdlog.h>

SyncProcess::SyncProcess(std::initializer_list<const char *> _args)
    : args(_args)
{
}

SyncProcess::SyncProcess(std::string _cmd, std::vector<std::string> _args)
{
  this->args.push_back(_cmd.c_str());
  for (auto &str : _args)
  {
    this->args.push_back(str.c_str());
  }
}

bool SyncProcess::run()
{
  int status = -1;
  std::error_code ec;

  reproc::options options;
  options.redirect.parent = true;
  options.deadline = reproc::milliseconds(10000);

  const char *args[this->args.size()];
  for (u_int16_t i = 0; i < this->args.size(); i++)
  {
    args[i] = this->args[i];
  }

  std::tie(status, ec) = reproc::run(args, options);

  if (ec)
  {
    spdlog::error(ec.message());
  }

  return !(ec ? ec.value() : status);
}
