#pragma once
#include <vector>
#include <memory>
#include "Bullet.hpp"
#include "events/Observer.hpp"
#include "PlayState.hpp"


class PlayState;
class WeaponSystem final: public Observer {
public:
    void onNotify(const std::shared_ptr<GameEvent>&) override;

    WeaponSystem(PlayState *playState);

private:
    using BulletPtr = std::unique_ptr<Bullet>;
    std::vector<BulletPtr> bullets;
    PlayState* playState;


};
