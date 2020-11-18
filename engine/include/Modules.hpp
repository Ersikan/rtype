#ifndef MODULES_HPP
#define MODULES_HPP

#include "components/Transform.hpp"
#include <vector>
#include <string>
#include <optional>

// General Module Interface

class IModule {
public:
    virtual void update() = 0;
    virtual ~IModule() {};
};

// Specific modules

enum class Input {
    LeftClick,
    RightClick,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Escape,
    LControl,
    LShift,
    LAlt,
    RControl,
    RShift,
    RAlt,
    Tab,
    Space,
    Enter,
    BackSpace,
    Left,
    Right,
    Up,
    Down,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15
};

class IDisplayModule : public IModule {
public:
    virtual void drawSprite(const std::string &name, Transform const &transform, unsigned tile_id) = 0;
};

class IInputModule : public IModule {
public:

    virtual std::vector<Input> getInputEvents() = 0;
    virtual Dimensional getCursorLocation() = 0;
};

class IAudioModule : public IModule {
public:
    virtual void playSound(const std::string &name, float volume, float pitch) = 0;
};

class INetworkModule : public IModule {
public:
    virtual long initInstance(unsigned maxPlayers) = 0;
    //virtual std::optional<Instance> getInstance();
    //TODO: Créer la classe instance et l'implémenter ici (voir l'UML)'
};

#endif