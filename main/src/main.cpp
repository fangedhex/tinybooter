#include <App.h>
#include <signal.h>
#include <functional>

// Very nice :shitty: code but required to work with signal handling to a class
std::function<void(int)> signalCallback;
void signalHandler(int signal)
{
  signalCallback(signal);
}

int main(int argc, char **argv)
{
  App *app;

  signalCallback = std::bind(&App::signalHandler, app, std::placeholders::_1, [](int status) {
    exit(status);
  });

  // Registers signals handling to our main class
  signal(SIGINT, &signalHandler);
  signal(SIGTERM, &signalHandler);

  auto exitResult = app->run(argc, argv);

  delete app;
  return exitResult;
}
