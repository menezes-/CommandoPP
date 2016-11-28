#pragma once
#include "events/GameEvent.hpp"
#include <vector>
#include <memory>
#include <deque>
#include <set>
#include <Game.h>
#include "Bullet.hpp"
#include "Joe.hpp"
#include "Dude.hpp"
#include <random>

class EntityManager final: public Observer {
    using alive_t = std::set<Entity *>;
public:
    using iterator = alive_t::iterator;
    using const_iterator = alive_t::const_iterator;

    void onNotify(const std::shared_ptr<GameEvent> &event) override;

    /**
     * Constroi o entity manager e constroi o Joe
     * @param eventDispatcher eventDispatcher necessário para construir o objeto Joe
     */
    EntityManager(EventDispatcher *eventDispatcher);

    /**
     * Gera `count` balas e as coloca no cache
     * @param count número de balas a ser geradas
     */
    void generateBullets(std::size_t count);

    /**
     * Função factory que constroí uma entidade (derivada de Entity) com os parametros do construtor
     * especificados em `params`
     * @tparam T Classe derivada de Entity
     * @tparam P Parametros do construtor da classe derivada
     * @param params Parametros do construtor da classe derivada
     * @return ponteiro para entidade criada
     */
    template<
        class T,
        class... P>
    inline T *makeEntity(P &&... params) {
        auto entity = std::unique_ptr<Entity>(new T(std::forward<P>(params)...));
        T *e_ptr = static_cast<T *>(entity.get());
        entities.push_back(std::move(entity));
        return e_ptr;
    }

    Dude* makeDude();

    void update(const sf::FloatRect &viewRect);

    Joe *getJoe() const;

    std::size_t size() const;

    iterator begin() { return alive.begin(); }

    iterator end() { return alive.end(); }

    const_iterator begin() const { return alive.begin(); }

    const_iterator end() const { return alive.end(); }

    const_iterator cbegin() const { return alive.cbegin(); }

    const_iterator cend() const { return alive.cend(); }

    static constexpr std::size_t bulletCacheSize = 1024;

private:
    alive_t alive;
    std::vector<std::unique_ptr<Entity>> entities;
    std::deque<Bullet *> bulletCache;

    void fireEvent(const std::shared_ptr<GameEvent> &event);

    void entityIsDeadEvent(const std::shared_ptr<GameEvent> &event);

    void gamePauseEvent(const std::shared_ptr<GameEvent> &event);
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;

    Joe *joe;

};
