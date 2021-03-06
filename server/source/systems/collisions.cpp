#include "Game.hpp"
#include "components/Transform.hpp"
#include "components/CollisionBox.hpp"
#include "components/GameObject.hpp"
#include "components/Destructible.hpp"
#include "components/WormHole.hpp"
#include "components/PlayerBarracks.hpp"
#include "components/Invulnerable.hpp"
#include "LevelState.hpp"
#include <algorithm>
#include <memory>

static bool are_colliding(Transform transform_a, CollisionBox box_a, \
Transform transform_b, CollisionBox box_b) {
    float origin_a_x = transform_a.location.x + box_a.offset_x * transform_a.scale.x;
    float origin_a_y = transform_a.location.y + box_a.offset_y * transform_a.scale.y;
    float bound_a_x = origin_a_x + box_a.width * transform_a.scale.x;
    float bound_a_y = origin_a_y + box_a.height * transform_a.scale.y;
    float origin_b_x = transform_b.location.x + box_b.offset_x * transform_b.scale.x;
    float origin_b_y = transform_b.location.y + box_b.offset_y * transform_b.scale.y;
    float bound_b_x = origin_b_x + box_b.width * transform_b.scale.x;
    float bound_b_y = origin_b_y + box_b.height * transform_b.scale.y;

    return !(
        origin_b_x >= bound_a_x || origin_a_x >= bound_b_x
        ||
        origin_b_y >= bound_a_y || origin_a_y >= bound_b_y
    );
}

void collisions_update(Game &instance)
{
    auto collision_parameters = instance.componentStorage.join_components(
        instance.componentStorage.getComponents<CollisionBox>(),
        instance.componentStorage.getComponents<Transform>(),
        instance.componentStorage.getComponents<GameObject>()
    );

    for (
        auto element = collision_parameters.begin();
        element != collision_parameters.end();
        element++
    ) {
        for (
            auto other = element;
            ++other != collision_parameters.end();
        ) {
            auto &[box, transform, type] = element->second;
            auto &[o_box, o_transform, o_type] = other->second;
            if (are_colliding(transform, box, o_transform, o_box)) {
                box.collidingWith = o_type;
                box.colliderRoughness = o_box.roughness;
                o_box.collidingWith = type;
                o_box.colliderRoughness = box.roughness;
            }
        }
    }
};

void invulnerability_applicator(CollisionBox &box, Invulnerable &invulnerable)
{
    if (invulnerable.ticks > 0) {
        box.collidingWith = {};
        invulnerable.ticks--;
    }
}

void collision_damages(CollisionBox &box, Destructible &destructible)
{
    if (box.collidingWith &&
std::find(box.ignoreList.begin(), box.ignoreList.end(), box.collidingWith.value())
== box.ignoreList.end()) {
        destructible.health -= box.colliderRoughness;
        box.collidingWith = {};
        if (destructible.health > 0)
            return;
        if (destructible.hasDeathMontage)
            destructible.status = Destructible::Status::Dying;
        else
            destructible.status = Destructible::Status::Dead;
    }
}

void collision_wormholes(Game &instance)
{
    auto hole_params = instance.componentStorage.join_components(
        instance.componentStorage.getComponents<WormHole>(),
        instance.componentStorage.getComponents<CollisionBox>()
    );
    auto &barracks = instance.componentStorage.getComponents<PlayerBarracks>();

    for (auto &[id, params] : hole_params) {
        auto &[hole, box] = params;
        if (box.collidingWith && box.collidingWith.value() == GameObject::PlayerShip) {
            auto &barrack = barracks.at(hole.barracks_id);
            std::unique_ptr<AState> level_state(new LevelState(barrack.playerConnected));
            barrack.active = false;
            instance.componentStorage.destroyEntity(hole.barracks_id);
            instance.stateMachine.setState(std::move(level_state));
        }
    }
}
