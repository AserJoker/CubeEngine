#pragma once
#include "ConfigManager.hpp"
#include "LocaleManager.hpp"
#include "Window.hpp"
#include "core/Object.hpp"
#include "core/Version.hpp"
#include "runtime/AssetManager.hpp"
#include "runtime/Logger.hpp"
#include "runtime/ModManager.hpp"
#include "runtime/ResourceManager.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

namespace cube::runtime {
class Application : public core::Object {
private:
  std::unordered_map<std::string, std::string> _options;
  runtime::Logger *_logger = runtime::Logger::getLogger("Application");
  std::unordered_map<SDL_WindowID, std::shared_ptr<Window>> _windows;
  bool _running = false;
  std::shared_ptr<ResourceManager> _resource;
  std::shared_ptr<ConfigManager> _config;
  std::shared_ptr<AssetManager> _asset;
  std::shared_ptr<LocaleManager> _locale;
  std::shared_ptr<ModManager> _mod;

  std::string _appname = "cube";
  std::string _appversion = "0.1.0";
  std::string _appid = "";

  std::ofstream _logfile;
  core::Version _version;

private:
  void initOptions(int argc, char *argv[]);
  void initLogger();
  void initLocale();

public:
  Application();
  bool hasOption(const std::string &name) const;
  const std::string &getOption(const std::string &name,
                               const std::string &def = "") const;
  int run(int argc, char *argv[]);
  ~Application() override;
  const char *getBasePath() const;
  Application *setApplicationInfo(const std::string &appname,
                                  const std::string &appversion,
                                  const std::string &appid);
  const std::string &getApplicationName() const;
  const core::Version &getApplicationVersion() const;
  Application *addWindow(const std::shared_ptr<Window> &window);
  bool removeWindow(SDL_WindowID id);
  std::shared_ptr<Window> getWindow(SDL_WindowID id) const;
};
}; // namespace cube::runtime