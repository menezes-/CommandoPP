#include <systems/RespawnSystem.hpp>
#include <events/Event.hpp>
#include <events/MObjectInView.hpp>
#include <algorithm>


void RespawnSystem::onNotify(const std::shared_ptr<GameEvent> &event) {
    auto eventType = event->getEvent();
    if (eventType == ENTITY_IS_DEAD) entityIsDead(event);
    if (eventType != Event::MAP_OBJECT_IN_VIEW) return;
    const MObjectInView *objectInView = static_cast<const MObjectInView *>(event.get());
    auto object = objectInView->getMapObject();
    auto name = object->GetName();
    if (name == "hero" || name == "respawn") {

        auto result = std::find(respawnPoints.begin(), respawnPoints.end(), object);
        if (result == respawnPoints.end()) {
            respawnPoints.push_back(object);
        }
    }

}


void RespawnSystem::entityIsDead(const std::shared_ptr<GameEvent> &event) {
    auto from = event->getFrom();
    if (from->getType() != JOE) return;
    respawn();

}


RespawnSystem::RespawnSystem(const EntityManager &entityManager)
    : joe{*entityManager.getJoe()} {}


void RespawnSystem::respawn() {
    if (joe.getLives() > 0) {

        // ordenação descendente
        std::sort(respawnPoints.begin(), respawnPoints.end(), [](const tmx::MapObject *a, const tmx::MapObject *b) {
            return b->GetCentre().y > a->GetCentre().y;
        });
        auto jy = joe.getPosition().y;
        auto result =
            std::find_if(respawnPoints.begin(), respawnPoints.end(), [jy](const tmx::MapObject *mapObject) {
                return jy < mapObject->GetCentre().y;
            });

        if (result == respawnPoints.end()) {
            joe.setPosition(respawnPoints.front()->GetCentre());
        } else {
            joe.setPosition((*result)->GetCentre());
        }

        joe.setVisible(true);
        joe.setState(EntityState::ALIVE);
        joe.setAndPlay("Up");
    }

}
