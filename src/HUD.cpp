#include "HUD.hpp"


HUD::HUD(const EntityManager &entityManager, const bool &pauseObserver)
    : joe(*entityManager.getJoe()), pauseObserver(pauseObserver) {

    font.loadFromFile("resources/fonts/Munro.ttf");
    iconTexture.loadFromFile("resources/sprites/pixel_icons_by_oceansdream.png");
    livesText.setFont(font);
    livesText.setCharacterSize(20);
    livesText.setPosition(spriteWidth + 2, 0);
    livesText.setFillColor(sf::Color(255, 255, 255));
    healthText.setFont(*livesText.getFont());
    healthText.setCharacterSize(livesText.getCharacterSize());
    healthText.setFillColor(livesText.getFillColor());
    pausedText.setFont(*livesText.getFont());
    pausedText.setCharacterSize(livesText.getCharacterSize());
    pausedText.setFillColor(livesText.getFillColor());
    pausedText.setString("Game Paused");

}


void HUD::draw(sf::RenderWindow *window) {
    if (pauseObserver && pausedClock.getElapsedTime() > sf::seconds(1)) {
        if (pausedClock.getElapsedTime() > sf::seconds(2.5)) {
            pausedClock.restart();
        }
        window->draw(pausedText);

    } else {
        auto currWeapon1 = joe.weapon.getCurrWeapon();
        if (currWeapon1 != currWeapon) {
            auto index = weaponSpriteIndex[currWeapon1];
            sf::IntRect rect{index.x * spriteWidth, index.y * spriteHeight, spriteWidth, spriteHeight};
            weaponSprite = sf::Sprite{iconTexture, rect};
            currWeapon = currWeapon1;
        }
        window->draw(weaponSprite);
        window->draw(livesText);
        window->draw(healthText);
    }
}


void HUD::update() {
    if (!pauseObserver) {
        livesText.setString(std::string{"Lives: "} + std::to_string(joe.getLives()));
        healthText
            .setPosition(livesText.getPosition().x + livesText.getLocalBounds().width + 10, livesText.getPosition().y);
        healthText.setString(std::string{"Health: "} + std::to_string(joe.getHealth()));
    }

}


