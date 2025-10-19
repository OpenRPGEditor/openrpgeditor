#pragma once

#include <string>

#include "nlohmann/json.hpp"

struct Plugin {
  friend void to_json(nlohmann::ordered_json& to, const Plugin& plugin);
  friend void from_json(const nlohmann::ordered_json& from, Plugin& plugin);

  std::string name;
  bool status{true};
  std::string description;
  std::vector<std::pair<std::string, std::string>> parameters;
};

struct Plugins {
  struct Context {
    struct Component {
      std::string englishComments;
      std::string localComments;
    };

    struct ParamType {
      std::string type;
      double min;
      double max;
      double decimals;
      std::string dir;
      bool require;
      std::string on;
      std::string off;
      std::vector<std::pair<std::string, std::string>> options;
    };
    std::string pluginHelp;
    std::string pluginDesc;
    std::string pluginAuthor;
    std::map<std::string, Component> components;
    std::vector<std::string> paramNames;
    std::map<std::string, std::string> paramDescs;
    std::map<std::string, std::string> paramDefaults;
    std::map<std::string, std::string> paramParents;
    std::map<std::string, std::string> paramTexts;
    std::map<std::string, ParamType> paramTypes;
    std::vector<std::string> requiredAssets;
    std::vector<std::string> noteParamNames;
    std::map<std::string, bool> noteParamRequires;
    std::map<std::string, std::string> noteParamTypes;
    std::map<std::string, std::string> noteParamDirs;
    std::map<std::string, std::string> noteParamDatas;
  };

  static Plugins load(const std::filesystem::path& path);
  bool serialize(const std::filesystem::path& path) const;

  std::vector<Plugin> plugins;

  void processPluginScripts(const std::filesystem::path& path);

private:
  void processCommentBlock(Context& ctx, const std::string& commentBlock);
};
