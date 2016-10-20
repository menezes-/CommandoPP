#include "HUD.hpp"


HUD::HUD(const EntityManager &entityManager)
    : joe(*entityManager.getJoe()) {

    font.loadFromFile("resources/fonts/minecraft_evenings.otf");
    iconTexture.loadFromFile("resources/sprites/pixel_icons_by_oceansdream.png");
    wBack.setSize(sf::Vector2f{spriteWidth, spriteHeight});
    wBack.setPosition(0, 0);
    wBack.setFillColor(sf::Color(146, 141, 154, 0xB3));
    livesText.setFont(font);
    livesText.setCharacterSize(14);
    livesText.setPosition(spriteWidth + 2, 0);
    livesText.setFillColor(sf::Color(255, 255, 255, 0xB3));
    healthText.setFont(font);
    healthText.setCharacterSize(14);
    healthText.setFillColor(sf::Color(255, 255, 255, 0xB3));

}


void HUD::draw(sf::RenderWindow *window) {
    auto currWeapon1 = joe.weapon.getCurrWeapon();
    if (currWeapon1 != currWeapon) {
        auto index = weaponSpriteIndex[currWeapon1];
        sf::IntRect rect{index.x * spriteWidth, index.y * spriteHeight, spriteWidth, spriteHeight};
        weaponSprite = sf::Sprite{iconTexture, rect};
        currWeapon = currWeapon1;
    }

    window->draw(wBack);
    window->draw(weaponSprite);
    window->draw(livesText);
    window->draw(healthText);

}


void HUD::update() {
    livesText.setString(std::string{"Lives: "} + std::to_string(joe.getLives()));
    healthText
        .setPosition(livesText.getPosition().x + livesText.getLocalBounds().width + 10, livesText.getPosition().y);
    healthText.setString(std::string{"Health: "} + std::to_string(joe.getHealth()));

}


