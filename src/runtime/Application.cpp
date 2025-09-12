#include "runtime/Application.hpp"
#include "core/Singleton.hpp"
#include "runtime/Logger.hpp"
#include "runtime/ModManager.hpp"
#include "runtime/ResourceManager.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <exception>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <locale.h>
#endif

using namespace cube;
using namespace cube::runtime;
Application::Application() {
  _resource = core::Singleton<ResourceManager>::get();
  _config = core::Singleton<ConfigManager>::get();
  _asset = core::Singleton<AssetManager>::get();
  _locale = core::Singleton<LocaleManager>::get();
  _mod = core::Singleton<ModManager>::get();
}
void Application::initOptions(int argc, char *argv[]) {
  int idx = 1;
  while (idx < argc) {
    std::string arg = argv[idx];
    if (arg.starts_with("--")) {
      arg = arg.substr(2);
      auto pos = arg.find("=");
      if (pos == std::string::npos) {
        _options[arg] = "";
      } else {
        std::string name = arg.substr(0, pos);
        std::string value = arg.substr(pos + 1);
        _options[name] = value;
      }
    }
  }
}

void Application::initLogger() {
  SDL_SetLogOutputFunction(runtime::Logger::print, nullptr);
  auto &cfg = _config->query(std::format("{}:application.toml", _appname));
  auto &log = cfg.get("log");
  runtime::Logger::setPriorities(log.get("priority").getString("debug"));
  auto &file = log.get("file").getString();
  if (!file.empty()) {
    if (Logger::initLoggerFile(_logfile, file)) {
      SDL_SetLogOutputFunction(runtime::Logger::print, &_logfile);
    } else {
      SDL_SetLogOutputFunction(runtime::Logger::print, nullptr);
      _logger->error("Failed to initialize log file: {}", file);
    }
  } else {
    SDL_SetLogOutputFunction(runtime::Logger::print, nullptr);
  }
}

void Application::initLocale() {
  auto &cfg = _config->query(std::format("{}:application.toml", _appname));
  auto &system = cfg.getField("system");
  _locale->registerLocale("en_US",
                          std::format("{}:locale/en_US.lang", _appname));
  _locale->setDefaultLanguage("en_US");
  _locale->setLanguage(system.getField("language").getString("en_US"));
}

bool Application::hasOption(const std::string &name) const {
  return _options.contains(name);
}

const std::string &Application::getOption(const std::string &name,
                                          const std::string &def) const {
  if (_options.contains(name)) {
    return _options.at(name);
  }
  return def;
}

int Application::run(int argc, char *argv[]) {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#else
  setlocale(LC_ALL, "");
#endif
  initOptions(argc, argv);
  _resource->setDomain(_appname, getBasePath());
  initLogger();
  try {
    initLocale();
    if (!SDL_SetAppMetadata(_appname.c_str(), _appversion.c_str(),
                            _appid.c_str())) {
      _logger->error("Failed to set application metadata: {}", SDL_GetError());
      return -1;
    }
    _version = _appversion;
    if (_version.ext == "error") {
      _logger->error("Invalid application version: {}", _appversion);
      return -1;
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
      _logger->error("Failed to initialize sdl3: {}", SDL_GetError());
      return -1;
    }
    _window = SDL_CreateWindow(_locale->i18n("system.window.title").c_str(),
                               1024, 768, 0);
    if (!_window) {
      _logger->error("Failed to create window: {}", SDL_GetError());
      return -1;
    }
    _mod->load();
    _locale->setLanguage(_locale->getLanguage());

    SDL_SetWindowTitle(_window, _locale->i18n("system.window.title").c_str());
    _running = true;
    while (_running) {
      SDL_Event event;
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
          _running = false;
        } else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
          _running = false;
        }
      } else {
        SDL_Delay(4);
      }
    }
    return 0;
  } catch (std::exception &e) {
    _logger->error("{}", e.what());
    return -1;
  }
}

Application::~Application() {
  if (_window) {
    SDL_DestroyWindow(_window);
  }
  SDL_Quit();
}

const SDL_Window *Application::getWindow() const { return _window; }

const char *Application::getBasePath() const { return SDL_GetBasePath(); }

Application *Application::setApplicationInfo(const std::string &appname,
                                             const std::string &appversion,
                                             const std::string &appid) {
  _appname = appname;
  _appversion = appversion;
  _appid = appid;
  return this;
}
const std::string &Application::getApplicationName() const { return _appname; }

const core::Version &Application::getApplicationVersion() const {
  return _version;
}