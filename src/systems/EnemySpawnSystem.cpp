#include "systems/EnemySpawnSystem.hpp"
#include "events/MObjectInView.hpp"
#include "Dude.hpp"


void EnemySpawnSystem::onNotify(const std::shared_ptr<GameEvent> &event) {
    if (event->getEvent() != Event::MAP_OBJECT_IN_VIEW) return;
    const MObjectInView *inView = static_cast<const MObjectInView *>(event.get());

    auto mapObject = inView->getMapObject();
    if (mapObject->GetName() != "trooperGenerator") return;


    if (inView->isInView()) {
        mapObjects.insert(mapObject);

    } else {
        auto search = mapObjects.find(mapObject);
        if (search != mapObjects.end()) {
            mapObjects.erase(search);
        }
    }

}


void EnemySpawnSystem::update() {
    if (clock.getElapsedTime() >= timeout) {
        clock.restart();
        for (auto mapObject: mapObjects) {
            std::unique_ptr<Entity> dude{new Dude{*eventDispatcher}};
            dude->setPosition(mapObject->GetPosition());
            eventDispatcher->notify(make_event<AddEntity>(std::move(dude)));

        }
    }

}
