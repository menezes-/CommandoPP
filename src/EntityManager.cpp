#include "EntityManager.hpp"
#include <events/FireEvent.hpp>
#include <events/PlaySoundEvent.hpp>


void EntityManager::onNotify(const std::shared_ptr<GameEvent> &event) {
    auto event_type = event->getEvent();
    switch (event_type) {
        case Event::ENTITY_IS_DEAD:
            entityIsDeadEvent(event);
            break;
        case Event::FIRE:
            fireEvent(event);
            break;
        default:
            break;
    }

}


EntityManager::EntityManager(EventDispatcher *eventDispatcher) {
    auto entity = std::unique_ptr<Entity>(new Joe(EntityConfig{}, *eventDispatcher));
    Joe *e_ptr = static_cast<Joe *>(entity.get());
    entities.push_back(std::move(entity));
    joe = e_ptr;
}


void EntityManager::generateBullets(std::size_t count) {
    WeaponConfig basicConfig{};
    sf::Vector2f direction{0.f, 0.f};
    for (std::size_t i = 0; i < count; ++i) {
        auto bullet_ptr = makeEntity<Bullet>(*eventDispatcher,
                                             EntityType::DUDE,
                                             basicConfig,
                                             direction);
        bullet_ptr->setState(EntityState::DEAD);
        bulletCache.push_back(bullet_ptr);

    }

}


void EntityManager::fireEvent(const std::shared_ptr<GameEvent> &event) {

    const FireEvent *fire_event = static_cast<const FireEvent *>(event.get());


    if (bulletCache.empty()) {
        // cinquenta é o maior pente disponível
        // gera balas suficientes para esvaziar um pente de metralhadora
        generateBullets(50);
    }

    // tiro que vem do nada, normalmente usado para teste
    EntityType type;
    if (!fire_event->getFrom()) {
        type = DUDE;
    } else {
        type = fire_event->getFrom()->getType();
    }


    Bullet *bullet = bulletCache.front();
    bulletCache.pop_front();

    bullet->setPosition(fire_event->getPosition());

    bullet->reuseBullet(type, fire_event->getWeaponConfig(), fire_event->getDirection());

}


void EntityManager::entityIsDeadEvent(const std::shared_ptr<GameEvent> &event) {
    auto from = event->getFrom();
    if (from && from->getType() == EntityType::BULLET) {
        auto bulltet_ptr = static_cast<Bullet *>(from);
        bulletCache.push_back(bulltet_ptr);
    }
    if (from && from->getType() == EntityType::JOE) {
        eventDispatcher->notify(make_event<PlaySoundEvent>(GameSound::DEATH_SOUND));
    }

}


void EntityManager::gamePauseEvent(const std::shared_ptr<GameEvent> &event) {

}


void EntityManager::update(const sf::FloatRect &viewRect) {
    alive.clear();
    for (const auto &entity: entities) {

        if (entity->getState() == DEAD) {
            continue;
        }
        if (!viewRect.contains(entity->getPosition())) {
            if (entity->getType() == BULLET) {
                Bullet *b = static_cast<Bullet *>(entity.get());
                b->die();
            }
            continue;
        }
        alive.insert(entity.get());
    }

}


std::size_t EntityManager::size() const {
    return alive.size();
}


Joe *EntityManager::getJoe() const {
    return joe;
}
