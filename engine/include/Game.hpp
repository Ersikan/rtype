#ifndef GAME_HPP
#define GAME_HPP

#include "StateMachine.hpp"
#include "ComponentStorage.hpp"
#include "Modules.hpp"
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include <functional>

class Game {
private:
    std::string m_name;
    std::unordered_map<std::string, std::unique_ptr<IModule>> m_modules;
public:
    StateMachine stateMachine;
    ComponentStorage componentStorage;
public:
    Game(std::string name);
    void run();

/*
   MODULES
*/
    void addModule(std::string name, std::unique_ptr<IModule> module);
    IModule &getModule(const std::string &name);
// Specific modules slots
    std::optional<std::reference_wrapper<IDisplayModule>> displayModule;
    std::optional<std::reference_wrapper<IInputModule>> inputModule;
    std::optional<std::reference_wrapper<IAudioModule>> audioModule;
    std::optional<std::reference_wrapper<INetworkModule>> networkModule;
// Specific modules sots setters
    void setDisplayModule(const std::string &name);
    void setInputModule(const std::string &name);
    void setAudioModule(const std::string &name);
    void setNetworkModule(const std::string &name);
};

#endif