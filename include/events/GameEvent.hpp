#pragma once
#include <memory>
#include <utility>
#include "Entity.hpp"
#include "Event.hpp"
class Entity;

class GameEvent {
public:
    GameEvent(Entity *from, Entity *target, Event event);

    GameEvent(Entity *from, Event event);

    GameEvent(Event event);

    Event getEvent() const;

    Entity *getFrom() const;

    Entity *getTarget() const;

    virtual ~GameEvent() = default;

    GameEvent() = default;

    GameEvent(const GameEvent &copyFrom) = default;

    GameEvent &operator=(const GameEvent &copyFrom) = default;

    GameEvent(GameEvent &&) = default;

    GameEvent &operator=(GameEvent &&) = default;

protected:
    Event event;

    //Caso o evento se refira a alguma entidade
    Entity *from{nullptr};
    Entity *target{nullptr};

};


/**
 * Foi escolhido um shared_ptr pois não é possível saber se
 * alguns dos observers vai querer copiar ou não o parametro do evento
 *
 * @tparam T classe derivada de GameEvent
 * @tparam P parametros do evento
 * @param params parametros do evento
 * @return argumento para um evento
 */
template<class T, class... P>
inline std::shared_ptr<GameEvent> make_event(P &&... params) {
    return std::make_shared<T>(std::forward<P>(params)...);
}


