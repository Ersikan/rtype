#include "Enemies.hpp"
#include "components/Destructible.hpp"
#include "components/Speaker.hpp"
#include <cmath>

const Enemy Enemy::BUG = {
    CollisionBox{
        27, 25, 4, 5, 1},
    Trajectory{[](Transform &t) {
        t.location.x -= 1.5;
        t.location.y = std::cos(t.location.x * 0.01) * 100 + 300;
    }},
    OrientedSprite("bug", std::array<unsigned, 16>{0, 1, 2, 3, 4, 5, 6, 7, 10,
                                                   11, 12, 13, 14, 15, 16, 17}),
    BydoShooter(),
    PlayerScanner(1000),
    1, DeathMontage("explosions", {0, 1, 2, 3, 4, 5}, 7),
    DeathSpeaker("enemy-explosion", 0.5, 0.5)
};

const Enemy Enemy::PATA_PATA = {
    CollisionBox{
        21, 20, 4, 2, 1},
    PatternLoop{{Pattern{{-2, 2}, 60}, Pattern{{-2, -2}, 60}}},
    AnimationLoop{{{"enemy_flap", 0},
                   {"enemy_flap", 1},
                   {"enemy_flap", 2},
                   {"enemy_flap", 3},
                   {"enemy_flap", 4},
                   {"enemy_flap", 5},
                   {"enemy_flap", 6},
                   {"enemy_flap", 7}},
                  15},
    BydoShooter(300, 0.7, {0, -1}, 0.6),
    PlayerScanner(1500),
    1, DeathMontage("explosions", {0, 1, 2, 3, 4, 5}, 7),
    DeathSpeaker("enemy-explosion", 0.5, 0.5)
};

const Enemy Enemy::TURRET_UP = {
    CollisionBox{
        18, 16, 0, -1, 1},
    std::nullopt,
    OrientedSprite{"blaster", std::array<unsigned, 10>{15, 14, 13, 12, 11, 10, 0, 0, 0, 0}},
    BydoShooter(300, 0.7, {0, -1}, 0.6),
    PlayerScanner(1500),
    1, DeathMontage("explosions", {0, 1, 2, 3, 4, 5}, 7),
    DeathSpeaker("enemy-explosion", 0.5, 0.5),
    TurretSprite{TurretSprite::UP}
};

const Enemy Enemy::TURRET_DOWN = {
    CollisionBox{
        18, 17, 0, 0, 1},
    std::nullopt,
    OrientedSprite{"blaster", std::array<unsigned, 10>{5, 0, 0, 0, 0, 0, 1, 2, 3, 4}},
    BydoShooter(300, 0.7, {0, -1}, 0.6),
    PlayerScanner(1500),
    1, DeathMontage("explosions", {0, 1, 2, 3, 4, 5}, 7),
    DeathSpeaker("enemy-explosion", 0.5, 0.5),
    TurretSprite{TurretSprite::DOWN}
};


void Enemy::build(ComponentStorage::EntityBuilder &builder) const
{
    if (auto animation = std::get_if<AnimationLoop>(&this->animation)) {
        builder.withComponent(animation->frames[0]);
    }
    if (auto orientation = std::get_if<OrientedSprite>(&this->animation)) {
        builder.withComponent(
            Sprite{orientation->sprite, orientation->tiles[0].tile_id});
    }
    if (this->movement.has_value()) {
        if (std::holds_alternative<PatternLoop>(*this->movement)) {
            builder.withComponent(Velocity{0, 0});
        }
        builder.withComponent(*this->movement);
    }
    if (this->turret)
        builder.withComponent(*this->turret);
    auto collision = this->collision;
    collision.ignoreList = {GameObject::Enemy, GameObject::EnemyProjectile,
                            GameObject::Wall};
    builder.withComponent(collision)
        .withComponent(this->animation)
        .withComponent(Destructible{this->health, true})
        .withComponent(this->death_montage)
        .withComponent(GameObject::Enemy)
        .withComponent(this->death_sound)
        .withComponent(this->shooter)
        .withComponent(this->scanner);
}
