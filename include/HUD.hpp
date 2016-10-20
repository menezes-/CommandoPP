#pragma once
#include <SFML/Graphics.hpp>
#include "Joe.hpp"
#include "EntityManager.hpp"

class HUD {
public:
    HUD(const EntityManager &entityManager);

    void draw(sf::RenderWindow *window);

    void update();

private:
    sf::Texture iconTexture;
    const Joe& joe;
    Weapons currWeapon{WEAPON_COUNT};
    sf::Sprite weaponSprite;
    static constexpr int spriteWidth{24};
    static constexpr int spriteHeight{24};
    std::array<sf::Vector2i, Weapons::WEAPON_COUNT> weaponSpriteIndex{{
                                                                    {0, 0},
                                                                    {2, 3},
                                                                    {3, 1},
                                                                    {0, 1},
                                                                }};

    sf::RectangleShape wBack;
    sf::Font font;
    sf::Text livesText;
    sf::Text healthText;


};
