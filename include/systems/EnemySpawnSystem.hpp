#pragma once
#include <events/Observer.hpp>
#include <SFML/System.hpp>
#include <set>
#include "tmx/MapObject.h"



class EnemySpawnSystem final: public Observer {
public:
    void onNotify(const std::shared_ptr<GameEvent> &event) override;
    void update();


private:
    std::set<const tmx::MapObject*> mapObjects{};
    sf::Clock clock;
    sf::Time timeout = sf::seconds(30);
};
