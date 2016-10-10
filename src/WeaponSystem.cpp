#include "WeaponSystem.hpp"
#include "events/FireEvent.hpp"


void WeaponSystem::onNotify(const std::shared_ptr<GameEvent>& event) {
    if (event->getEvent() != FIRE) return;
    const FireEvent *fire_event = static_cast<const FireEvent *>(event.get());

    // tiro que vem do nada, normalmente usado para teste
    EntityType type;
    if (!fire_event->getFrom()) {
        type = DUDE;
    } else {
        type = fire_event->getFrom()->getType();
    }

    auto bullet_ptr =
        std::unique_ptr<Bullet>(new Bullet(*eventDispatcher, type, fire_event->getWeaponConfig(), fire_event->getDirection()));
    if(fire_event->getFrom()){
        bullet_ptr->setPosition(fire_event->getFrom()->getPosition());
    }

    playState->addEntity(bullet_ptr.get());
    bullets.push_back(std::move(bullet_ptr));
}


WeaponSystem::WeaponSystem(PlayState *playState)
    : playState(playState), bullets{1024} {}
