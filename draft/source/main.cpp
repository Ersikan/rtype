#include "Game.hpp"
#include "SfmlAudioModule.hpp"
#include "SfmlModule.hpp"
#include "components/Sprite.hpp"
#include <iostream>
#include <boost/dll/runtime_symbol_info.hpp>

class BasicState : public AState {
public:
    void onStart(Game &instance) {}
    void onPause(Game &instance) {}
    void onResume(Game &instance) {}
    void onTick(Game &instance) {}
    void onStop(Game &instance) {}
};

struct PlayerControlled{};

int main(void)
{
    Game game("R-Type SOLO");
    std::filesystem::path assets_location =
        std::filesystem::path(boost::dll::program_location().string())
            .parent_path()
            .parent_path()
            .parent_path()
            .append("draft")
            .append("assets");
    std::unique_ptr<IModule> sfml_module(new SfmlModule(
        "R-Type SOLO v0.1",
        assets_location));
    std::unique_ptr<AState> state(new BasicState);
    auto audio_module = std::make_unique<SfmlAudioModule>();

    game.addModule("sfml", std::move(sfml_module));
    game.setDisplayModule("sfml");
    game.setInputModule("sfml");
    game.addModule("audio-sfml", std::move(audio_module));
    game.setAudioModule("audio-sfml");

    game.componentStorage.registerComponent<Transform>();
    game.componentStorage.registerComponent<Sprite>();
    game.componentStorage.registerComponent<PlayerControlled>();

    // System that displays entities with a transform and a sprite on screen
    game.systemStorage.addSystem([](IDisplayModule &display,
                                    const Transform &transform,
                                    const Sprite &sprite) {
                display.drawSprite(sprite.name, transform, sprite.tile_id);
    });

    // System that moves player controlled entities
    game.systemStorage.addSystem([](IInputModule &input, PlayerControlled p, Transform &transform) {
        if (input.isKeyPressed(Input::Up))
            transform.location.y--;
        if (input.isKeyPressed(Input::Down))
            transform.location.y++;
        if (input.isKeyPressed(Input::Left))
            transform.location.x--;
        if (input.isKeyPressed(Input::Right))
            transform.location.x++;
    });

    game.stateMachine.setState(std::move(state));

    game.componentStorage.buildEntity()
    .withComponent(Sprite("player_ships", 0))
    .withComponent(Transform(Dimensional(10, 10), Dimensional(10, 10), Dimensional(10, 10)))
        .withComponent(PlayerControlled{})
    .build();

    game.run();
}
