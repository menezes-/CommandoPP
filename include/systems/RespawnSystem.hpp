#pragma once
#include <vector>
#include "tmx/MapObject.h"
#include <EntityManager.hpp>


class RespawnSystem final: public Observer {
public:
    void onNotify(const std::shared_ptr<GameEvent> &event) override;
    void respawn();

    RespawnSystem(const EntityManager &entityManager);

private:
    std::vector<const tmx::MapObject *> respawnPoints;
    Joe& joe;

    void entityIsDead(const std::shared_ptr<GameEvent> &event);
};
