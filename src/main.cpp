#pragma once

#include <SFML/Graphics.hpp>
#include "player.cpp"
#include "enemy.cpp"
#include "coins.cpp"
#include "powerups.cpp"
#include "sounds.cpp"
#include <iostream>


int main()
{
    auto window = sf::RenderWindow{ { 600u, 800u }, "Game", sf::Style::Close | sf::Style::Titlebar };
    window.setFramerateLimit(144);

    // Font
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    // Score text
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    // Crosshair
    window.setMouseCursorVisible(false);
    sf::Texture crosshairTexture;
    crosshairTexture.loadFromFile("assets/sprites/crosshair.png");
    sf::Sprite crosshairSprite;
    crosshairSprite.setTexture(crosshairTexture);
    sf::Vector2u crosshairSize = crosshairTexture.getSize();
    crosshairSprite.setOrigin(crosshairSize.x / 2, crosshairSize.y / 2);
    crosshairSprite.setScale(0.4f, 0.4f);

    // Entities
    Player player(window.getSize().x, window.getSize().y);
    std::vector<Enemy> enemies;
    std::vector<Coins> coins;
    std::vector<Powerups> powerups;
    std::vector<Powerups> powerupsActive;

    Sounds sounds;
    sf::Clock clock;

    bool isMousePressed = false;
    bool isGameOver = false;
    int score = 0;
    int shots = 0;
    int hits = 0;
    int crosshairMultiplier = 1;
    int scoreMultiplier = 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Mouse clicks
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (!isMousePressed && !isGameOver) {
                    isMousePressed = true;
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

                    for (size_t i = 0; i < enemies.size(); i++) {
                        if (enemies[i].isOnTarget(crosshairSprite)) {
                            enemies.erase(enemies.begin() + i);
                            sounds.playHitSound();
                            score += 10 * scoreMultiplier;
                            hits++;
                            break;
                        }
                    }
                    shots++;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isMousePressed = false;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                if (isGameOver) {
                    isGameOver = false;
                    score = 0;
                    shots = 0;
                    hits = 0;
                    crosshairMultiplier = 1;
                    scoreMultiplier = 1;
                    player.reset(window.getSize().x, window.getSize().y);
                    enemies.clear();
                    coins.clear();
                    clock.restart();
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        // Player Movement
        player.move(deltaTime, window);

        // Powerups
        if (rand() % 500 < 1) {
            powerups.push_back(Powerups(window.getSize().x, window.getSize().y));
        }

        for (size_t i = 0; i < powerups.size(); i++) {
            powerups[i].move(deltaTime);

            if (!isGameOver && powerups[i].isColliding(player.shape)) {
                powerups[i].applyEffect(player, enemies, scoreMultiplier, crosshairMultiplier);
                powerups[i].printType();
                powerupsActive.push_back(powerups[i]);
                powerups.erase(powerups.begin() + i);
                sounds.playPowerupSound();
            }
            else if (powerups[i].isAtBottom(window.getSize().y)) {
                powerups.erase(powerups.begin() + i);
            }
        }

        for (size_t i = 0; i < powerupsActive.size(); i++) {
            powerupsActive[i].update(deltaTime);
            bool isExpired = powerupsActive[i].isExpired();
            bool hasNewerEffect = powerupsActive[i].hasNewerEffect(powerupsActive);
            if (isExpired && hasNewerEffect) {
                powerupsActive.erase(powerupsActive.begin() + i);
            }
            else if (isExpired && !hasNewerEffect) {
                powerupsActive[i].removeEffect(player, scoreMultiplier, crosshairMultiplier);
                powerupsActive.erase(powerupsActive.begin() + i);
            }
        }


        // Crosshair
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        float crosshairSpeed = 1.0f;
        crosshairSprite.setPosition(static_cast<float>(mousePosition.x) * crosshairSpeed, static_cast<float>(mousePosition.y) * crosshairSpeed);
        crosshairSprite.setScale(0.4f * crosshairMultiplier, 0.4f * crosshairMultiplier);


        // Enemy spawn
        if (rand() % 100 < 2) { // 2% chance to spawn a triangle each frame
            float x = static_cast<float>(rand() % window.getSize().x);
            enemies.push_back(Enemy(window.getSize().x, window.getSize().y));
        }


        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].move(deltaTime);

            // Check for collision or if enemy is at the bottom of the window
            if (!isGameOver && enemies[i].isCollidingWithPlayer(player.shape)) {
                enemies.erase(enemies.begin() + i);
                isGameOver = true;
            }
            else if (enemies[i].isAtBottom(window.getSize().y)) {
                enemies.erase(enemies.begin() + i);
                if (!isGameOver) {
                    score -= 100;
                }
            }
        }


        // Coin spawn
        if (rand() % 200 < 1) {
            float x = static_cast<float>(rand() % window.getSize().x);
            coins.push_back(Coins(window.getSize().x, window.getSize().y));
        }

        for (size_t i = 0; i < coins.size(); i++) {
            coins[i].move(deltaTime);

            // Check for collision or if enemy is at the bottom of the window
            if (!isGameOver && coins[i].isCollidingWithPlayer(player.shape)) {
                coins.erase(coins.begin() + i);
                sounds.playCoinSound();
                score += 500 * scoreMultiplier;
            }
            else if (coins[i].isAtBottom(window.getSize().y)) {
                coins.erase(coins.begin() + i);
            }
        }


        // Display
        float accuracy = shots == 0 ? 0 : static_cast<float>(hits) / static_cast<float>(shots);
        scoreText.setString("Score: " + std::to_string(score)
            + " Accuracy: " + std::to_string(static_cast<int>(accuracy * 100)) + "%");

        window.clear();
        window.draw(player.shape);
        window.draw(crosshairSprite);
        for (auto& enemy : enemies) {
            window.draw(enemy.shape);
        }
        for (auto& coin : coins) {
            window.draw(coin.shape);
        }
        for (auto& powerup : powerups) {
            window.draw(powerup.shape);
        }
        window.draw(scoreText);

        if (isGameOver) {
            sf::RectangleShape gameOverShape(sf::Vector2f(300, 120));
            gameOverShape.setFillColor(sf::Color::Black);
            gameOverShape.setOutlineColor(sf::Color::White);
            gameOverShape.setOutlineThickness(5);
            gameOverShape.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 100);
            sf::Text gameOverText("Game Over", font, 24);
            sf::Text finalScoreText("Final Score: " + std::to_string(score), font, 24);
            sf::Text playAgainText("Press R to play again", font, 24);
            gameOverText.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 - 80);
            playAgainText.setPosition(window.getSize().x / 2 - 120, window.getSize().y / 2 - 40);
            window.draw(gameOverShape);
            window.draw(gameOverText);
            window.draw(playAgainText);
        }

        window.display();
    }
}
