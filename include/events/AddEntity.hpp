#pragma once
#include "GameEvent.hpp"
#include <memory>

class AddEntity final: public GameEvent {
public:
    AddEntity(std::unique_ptr<Entity> entity);

    std::unique_ptr<Entity> getEntity() const;

private:
    std::unique_ptr<Entity> entity;

};
