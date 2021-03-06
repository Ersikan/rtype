#ifndef MODULES_HPP
#define MODULES_HPP

#include "components/Transform.hpp"
#include <vector>
#include <string>
#include <optional>
#include <stdexcept>

// General Module Interface

class IModule {
public:
    virtual void update() = 0;
    virtual ~IModule() {};
};

// Specific modules

enum class Input: uint8_t {
    Unknwown = 0,
    LeftClick = 1,
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
    F15,
    ExitWindow,
    KeyCount
};

class IDisplayModule : public virtual IModule {
public:
    virtual void drawSprite(const std::string &name, Transform const &transform, unsigned tile_id) = 0;
};

class IInputModule : public virtual IModule {
public:
    virtual Dimensional getCursorLocation(unsigned player) = 0;
    virtual bool isKeyPressed(unsigned player, Input key) = 0;
};

class IAudioModule : public virtual IModule
{
  public:
    virtual void playSound(const std::string &name, float volume = 1,
                           float pitch = 1, bool looping = false) = 0;
    virtual void stopSound(const std::string &name) = 0;
};

class AudioError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

#endif
