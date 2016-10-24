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

        /*
         * Os mapas são verticais de maneira que ao caminhar para a frente o jogador vai em direção
         * ao y = 0. A ideia é que o jogador renaça no ponto de respawn mais adiante considerando a posição
         * ATUAL dele. Outra maneira de pensar é: ponto de respawn mais proximo de onde o jogado morreu.
         * Primeiramente o array é ordenado de maneira ascendente (ou seja os pontos de respawn mais avançados aparecem
         * primeiro).
         * Depois é escolhido o primeiro ponto de respawn cujo Y é MAIOR que o Y atual do jogador. Como o array esta
         * ordenado do mais avançado para o menos o jogador é colocado no ponto de respawn mais avançado e mais proximo
         * pelo qual ele já passou.
         */
        std::sort(respawnPoints.begin(), respawnPoints.end(), [](const tmx::MapObject *a, const tmx::MapObject *b) {
            return a->GetCentre().y < b->GetCentre().y;
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
