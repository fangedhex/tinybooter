#include <ChildProcess.h>

#include <future>
#include <iostream>
#include <mutex>
#include <reproc++/drain.hpp>
#include <spdlog/spdlog.h>
#include <util/line_sink.h>
#include <thread>

ChildProcess::ChildProcess(std::string _cmd, std::vector<std::string> _args)
{
  this->argv.push_back(_cmd);
  this->argv.insert(this->argv.end(), _args.begin(), _args.end());
}

ChildProcess::~ChildProcess()
{
  this->stop();
}

static int fail(std::error_code ec)
{
  spdlog::error(ec.message());
  return ec.value();
}

bool ChildProcess::run()
{
  reproc::options options;

  std::error_code ec = process.start(this->argv, options);

  if (ec == std::errc::no_such_file_or_directory)
  {
    std::cerr << "Program not found. Make sure it's available from the PATH.";
    return ec.value();
  }
  else if (ec)
  {
    return fail(ec);
  }

  this->isRunning = true;

  // TODO It's NOT final as it needs to handle multi line and also stop from spamming console
  std::thread logThread([this]() {
    using namespace std::chrono_literals;

    std::string output;
    //reproc::sink::string sink(output);
    line_sink sink(argv[0]);
    std::error_code ec;

    do
    {
      reproc::drain(process, sink, reproc::sink::null);
      std::this_thread::sleep_for(400ms);
    } while (this->isRunning);
  });

  int status = 0;
  std::tie(status, ec) = process.wait(reproc::infinite);
  this->isRunning = false;

  if (ec)
  {
    return fail(ec);
  }

  logThread.join();

  return true;
}

std::future<bool> ChildProcess::runAsync() {
  auto func = std::bind(&ChildProcess::run, this);
  return std::async(std::launch::async, func);
}

int ChildProcess::stop()
{
  reproc::stop_actions stop = {
      {reproc::stop::terminate, reproc::milliseconds(5000)},
      {reproc::stop::kill, reproc::milliseconds(2000)},
      {}};

  std::error_code ec;
  int status = 0;
  std::tie(status, ec) = process.stop(stop);
  if (ec)
  {
    return fail(ec);
  }

  return status;
}
