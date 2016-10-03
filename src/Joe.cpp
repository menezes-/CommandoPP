#include "../include/Joe.hpp"


Joe::Joe(cgf::Game *gameObj, EntityConfig config, EventDispatcher &eventDispatcher)
    : Entity(gameObj, JOE, config, eventDispatcher) {}


void Joe::onEntityCollision(Entity &entity) {

}
