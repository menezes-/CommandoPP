#pragma once
#include <events/Observer.hpp>

class CollisionSystem final: public Observer {
public:
    void onNotify(const std::shared_ptr<GameEvent> &event) override;
};

