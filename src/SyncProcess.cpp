#include <SyncProcess.h>

#include <spdlog/spdlog.h>

SyncProcess::SyncProcess(std::string _cmd, std::vector<std::string> _args)
{
  this->argv.push_back(_cmd);
  this->argv.insert(this->argv.end(), _args.begin(), _args.end());
}

bool SyncProcess::run()
{
  int status = -1;
  std::error_code ec;

  reproc::options options;
  options.redirect.parent = true;
  options.deadline = reproc::milliseconds(10000);

  std::tie(status, ec) = reproc::run(this->argv, options);

  if (ec)
  {
    spdlog::error(ec.message());
  }

  return !(ec ? ec.value() : status);
}
